#pragma once

#include <chowdsp_wdf/chowdsp_wdf.h>

class CowbellEnvelope
{
public:
    CowbellEnvelope() = default;

    void setSnapPercent (float snapPercent01)
    {
        C9.setCapacitanceValue ((500.0f + 1250.0f * (1.0f - snapPercent01)) * 1.0e-9f);
    }

    void prepare (float sampleRate)
    {
        C9.prepare (sampleRate);
    }

    inline float processSample (float x) noexcept
    {
        Vin.setVoltage (x);

        D2.incident (I1.reflected());
        I1.incident (D2.reflected());

        return chowdsp::wdft::voltage<float> (C9);
    }

private:
    chowdsp::wdft::ResistorT<float> Rload { 20.0e3f };
    chowdsp::wdft::CapacitorT<float> C9 { 470.0e-9f };
    chowdsp::wdft::WDFParallelT<float, decltype (Rload), decltype (C9)> P1 { Rload, C9 };

    chowdsp::wdft::ResistorT<float> R2 { 1000.0f };
    chowdsp::wdft::WDFSeriesT<float, decltype (R2), decltype (P1)> S1 { R2, P1 };

    chowdsp::wdft::ResistiveVoltageSourceT<float> Vin;
    chowdsp::wdft::WDFSeriesT<float, decltype (S1), decltype (Vin)> S2 { S1, Vin };

    chowdsp::wdft::PolarityInverterT<float, decltype (S2)> I1 { S2 };
    chowdsp::wdft::DiodeT<float, decltype (I1)> D2 { I1, 2.64e-9f, 0.0265f, 1.5f };
};
