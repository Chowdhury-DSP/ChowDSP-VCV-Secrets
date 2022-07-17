#include "../plugin.hpp"
#include "CowbellEnvelope.h"

#include <chowdsp_dsp_utils/chowdsp_dsp_utils.h>
#include <chowdsp_waveshapers/chowdsp_waveshapers.h>

namespace {
    constexpr float highFreq = 2000.0f;
    constexpr float lowFreq = 850.0f;

    enum {
        ParamDivide = 16,
    };
}

struct ChowBell : Module {
    enum ParamIds {
        FREQ_PARAM,
        CUTOFF_PARAM,
        SNAPPY_PARAM,
        NUM_PARAMS
    };
    enum InputIds {
        TRIGGER_INPUT,
        NUM_INPUTS
    };
    enum OutputIds {
        MAIN_OUTPUT,
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    ChowBell() {
        config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);

        configInput(TRIGGER_INPUT, "Trigger");
        configOutput(MAIN_OUTPUT, "Cowbell");

        configParam(SNAPPY_PARAM, 0.0f, 1.0f, 0.5f, "Snap", "%");
        configParam(FREQ_PARAM, 0.0f, 1.0f, 0.5f, "Pitch", "%");
        configParam(CUTOFF_PARAM, 0.0f, 1.0f, 0.4f, "Cutoff", " Hz", highFreq / lowFreq, lowFreq);

        onSampleRateChange();
        paramDivider.setDivision(ParamDivide);
        adaaTanhClipper.prepare (1);
    }

    void onSampleRateChange() override {
        float newSampleRate = APP->engine->getSampleRate();
        envelope.prepare (newSampleRate);

        juce::dsp::ProcessSpec spec { (double) newSampleRate, 1, 1 };
        osc1.prepare (spec);
        osc2.prepare (spec);

        bandpassFilter.prepare (spec);
        bandpassFilter.setQValue (3.5f);

        cookParams();
    }

    void cookParams()
    {
        const auto freqParam = params[FREQ_PARAM].getValue();
        const auto freqMultFactor = std::pow (2.0f, 2.0f * (freqParam - 0.5f));
        osc1.setFrequency (800.0f * freqMultFactor);
        osc2.setFrequency (540.0f * freqMultFactor);

        const auto cutoffParam = params[CUTOFF_PARAM].getValue();
        const auto cutoffFreq = std::pow(highFreq / lowFreq, cutoffParam) * lowFreq;
        bandpassFilter.setCutoffFrequency (cutoffFreq);

        envelope.setSnapPercent (params[SNAPPY_PARAM].getValue());
    }

    void process(const ProcessArgs& args) override
    {
        if(paramDivider.process())
            cookParams();

        float x = inputs[TRIGGER_INPUT].getVoltage() * 0.1f;

        const auto env = envelope.processSample (x);
        const auto osc1Out = expLUT (osc1.processSample()) * env;
        const auto osc2Out = expLUT (osc2.processSample()) * env;

        auto y = adaaTanhClipper.processSample (osc1Out + osc2Out);
        y = bandpassFilter.processSample (0, y);

        outputs[MAIN_OUTPUT].setVoltage(y * 10.0f);
    }

    dsp::ClockDivider paramDivider;

    CowbellEnvelope envelope;

    chowdsp::SquareWave<float> osc1;
    chowdsp::SquareWave<float> osc2;

    chowdsp::SVFBandpass<> bandpassFilter;

    chowdsp::LookupTableTransform<float> expLUT { [] (auto x) { return std::exp (x); },
                                                  -1.1f,
                                                  1.1f,
                                                  1 << 12 };
    chowdsp::ADAATanhClipper<float> adaaTanhClipper;
};

struct ChowBellWidget : ModuleWidget {
    ChowBellWidget(ChowBell* module) {
        setModule(module);
        setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ChowBell.svg")));
//        createScrews(*this);

        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.0, 26.75)), module, ChowBell::FREQ_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.0, 45.75)), module, ChowBell::CUTOFF_PARAM));
        addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.0, 64.75)), module, ChowBell::SNAPPY_PARAM));

        addInput(createInputCentered<PJ301MPort>(mm2px(Vec(16.0, 97.5)), module, ChowBell::TRIGGER_INPUT));
        addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(16.0, 115.0)), module, ChowBell::MAIN_OUTPUT));
    }
};

Model* modelChowBell = createModel<ChowBell, ChowBellWidget>("ChowBell");
