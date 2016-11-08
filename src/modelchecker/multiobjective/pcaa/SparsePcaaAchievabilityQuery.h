#ifndef STORM_MODELCHECKER_MULTIOBJECTIVE_PCAA_SPARSEPCAAACHIEVABILITYQUERY_H_
#define STORM_MODELCHECKER_MULTIOBJECTIVE_PCAA_SPARSEPCAAACHIEVABILITYQUERY_H_

#include "src/modelchecker/multiobjective/pcaa/SparsePcaaQuery.h"

namespace storm {
    namespace modelchecker {
        namespace multiobjective {
            
            /*
             * This class represents a query for the Pareto curve approximation algorithm (Pcaa).
             * It implements the necessary computations for the different query types.
             */
            template <class SparseModelType, typename GeometryValueType>
            class SparsePcaaAchievabilityQuery : public SparsePcaaQuery<SparseModelType, GeometryValueType> {
            public:
                
                // Typedefs for simple geometric objects
                typedef std::vector<GeometryValueType> Point;
                typedef std::vector<GeometryValueType> WeightVector;
                
                /*
                 * Creates a new query for the Pareto curve approximation algorithm (Pcaa)
                 * @param preprocessorResult the result from preprocessing
                 */
                SparsePcaaAchievabilityQuery(SparsePcaaPreprocessorReturnType<SparseModelType>& preprocessorResult);
                
                
                /*
                 * Invokes the computation and retrieves the result
                 */
                virtual std::unique_ptr<CheckResult> check() override;
                
            private:
                
                void initializeThresholdData();
                
                /* 
                 * Returns whether the given thresholds are achievable.
                 */
                bool checkAchievability();
                
                /*
                 * Returns true iff there is one point in the given polytope that satisfies the given thresholds.
                 * It is assumed that the given polytope contains the downward closure of its vertices.
                 */
                bool checkIfThresholdsAreSatisfied(std::shared_ptr<storm::storage::geometry::Polytope<GeometryValueType>> const& polytope);

                
                Point thresholds;
                storm::storage::BitVector strictThresholds;
            };
            
        }
    }
}

#endif /* STORM_MODELCHECKER_MULTIOBJECTIVE_PCAA_SPARSEPCAAACHIEVABILITYQUERY_H_ */
