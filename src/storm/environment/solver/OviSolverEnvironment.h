#pragma once

#include "storm/environment/solver/SolverEnvironment.h"

#include "storm/adapters/RationalNumberAdapter.h"

namespace storm {
    
    class OviSolverEnvironment {
    public:
        
        OviSolverEnvironment();
        ~OviSolverEnvironment();
        
        storm::RationalNumber getPrecisionUpdateFactor() const;
        storm::RationalNumber getMaxVerificationIterationFactor() const;
        bool useRelevantValuesForPrecisionUpdate() const;
        
    private:
        storm::RationalNumber precisionUpdateFactor;
        storm::RationalNumber maxVerificationIterationFactor;
        bool relevantValuesForPrecisionUpdate;
    };
}

