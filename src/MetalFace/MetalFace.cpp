#include <filesystem>
#include <RTNeural/RTNeural.h>
#include <chowdsp_dsp_utils/chowdsp_dsp_utils.h>

#include "../plugin.hpp"

template <typename ModelType>
void loadLSTMModel (ModelType& model, int hiddenSize, const nlohmann::json& weights_json)
{
    using Vec2d = std::vector<std::vector<float>>;
    auto transpose = [] (const Vec2d& x) -> Vec2d
    {
        auto outer_size = x.size();
        auto inner_size = x[0].size();
        Vec2d y (inner_size, std::vector<float> (outer_size, 0.0f));

        for (size_t i = 0; i < outer_size; ++i)
        {
            for (size_t j = 0; j < inner_size; ++j)
                y[j][i] = x[i][j];
        }

        return y;
    };

    auto& lstm = model.template get<0>();
    auto& dense = model.template get<1>();

    Vec2d lstm_weights_ih = weights_json["/state_dict/rec.weight_ih_l0"_json_pointer];
    lstm.setWVals (transpose (lstm_weights_ih));

    Vec2d lstm_weights_hh = weights_json["/state_dict/rec.weight_hh_l0"_json_pointer];
    lstm.setUVals (transpose (lstm_weights_hh));

    std::vector<float> lstm_bias_ih = weights_json["/state_dict/rec.bias_ih_l0"_json_pointer];
    std::vector<float> lstm_bias_hh = weights_json["/state_dict/rec.bias_hh_l0"_json_pointer];
    for (int i = 0; i < 4 * hiddenSize; ++i)
        lstm_bias_hh[(size_t) i] += lstm_bias_ih[(size_t) i];
    lstm.setBVals (lstm_bias_hh);

    Vec2d dense_weights = weights_json["/state_dict/lin.weight"_json_pointer];
    dense.setWeights (dense_weights);

    std::vector<float> dense_bias = weights_json["/state_dict/lin.bias"_json_pointer];
    dense.setBias (dense_bias.data());
}

struct MetalFace : Module {
    enum ParamIds {
        METAL_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        MAIN_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        MAIN_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    MetalFace() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

        configInput(MAIN_INPUT, "Main");
        configOutput(MAIN_OUTPUT, "Main");

        configParam(METAL_PARAM, 0.0f, 1.0f, 0.5f, "Metal", "%");

        const auto modelFilePath = asset::plugin(pluginInstance, "res/metal_face_model.json");
        std::ifstream jsonStream(modelFilePath, std::ifstream::binary);
        nlohmann::json modelJson;
        jsonStream >> modelJson;
        loadLSTMModel(model, hiddenSize, modelJson);

        onSampleRateChange();
        paramDivider.setDivision(16);
    }

    void onReset() override {
        Module::onReset();

        model.reset();
    }

    void onSampleRateChange() override {
        float newSampleRate = APP->engine->getSampleRate();

        // we need to upsample if the sample rate is less than the training sample rate
        isUpsampling = newSampleRate < trainingSampleRate;

        upsampler.prepare ({ (double) newSampleRate, 1, 1 }, 2);
        downsampler.prepare ({ 2.0 * (double) newSampleRate, 2, 1 }, 2);

        
        const auto rnnSampleRate = newSampleRate * (isUpsampling ? 2.0f : 1.0f);
        const auto rnnDelaySamples = std::max (1.0f, rnnSampleRate / trainingSampleRate);

        model.template get<0>().prepare (rnnDelaySamples);
    }

    void process(const ProcessArgs& args) override {
        if(paramDivider.process())
            gain = rack::dsp::dbToAmplitude (-48.0f + 54.0f * params[METAL_PARAM].getValue());

        float x = inputs[MAIN_INPUT].getVoltage() * 0.1f * gain;

        float y = 0.0f;
        if (isUpsampling)
        {
            upsampler.process (&x, osData, 0, 1);
            for (int i = 0; i < osRatio; ++i)
                osData[i] = model.forward (&osData[i]);
            downsampler.process (osData, &y, 0, osRatio);
        }
        else
        {
            y = model.forward(&x);
        }

        outputs[MAIN_OUTPUT].setVoltage(y * 10.0f);
    }

    static constexpr auto DefaultSRCMode = RTNeural::SampleRateCorrectionMode::LinInterp;
    static constexpr int hiddenSize = 28;
    static constexpr float trainingSampleRate = 96000.0f;
    using RecurrentLayerType = RTNeural::LSTMLayerT<float, 1, hiddenSize, DefaultSRCMode>;
    using DenseLayerType = RTNeural::DenseT<float, hiddenSize, 1>;
    RTNeural::ModelT<float, 1, 1, RecurrentLayerType, DenseLayerType> model;

    dsp::ClockDivider paramDivider;
    float gain = 0.0f;

    using AAFilter = chowdsp::ButterworthFilter<8>;
    chowdsp::Upsampler<float, AAFilter> upsampler;
    chowdsp::Downsampler<float, AAFilter> downsampler;
    static constexpr int osRatio = 2;
    bool isUpsampling = true;
    float osData[2] {};
};

struct MetalFaceWidget : ModuleWidget {
    MetalFaceWidget(MetalFace* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/DummyPanel.svg")));
//        createScrews(*this);

        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.0, 26.75)), module, MetalFace::METAL_PARAM));

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(16.0, 97.5)), module, MetalFace::MAIN_INPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(16.0, 115.0)), module, MetalFace::MAIN_OUTPUT));
    }
};

Model* modelMetalFace = createModel<MetalFace, MetalFaceWidget>("MetalFace");
