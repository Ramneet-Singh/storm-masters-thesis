#ifndef STORM_MODELCHECKER_REACHABILITY_SPARSEDTMCREGIONMODELCHECKER_H_
#define STORM_MODELCHECKER_REACHABILITY_SPARSEDTMCREGIONMODELCHECKER_H_

#include <type_traits>

#include "src/storage/sparse/StateType.h"
#include "src/models/sparse/Dtmc.h"
#include "src/models/sparse/Mdp.h"
#include "src/utility/constants.h"
#include "utility/regions.h"
#include "src/solver/Smt2SmtSolver.h"
#include "src/modelchecker/reachability/SparseDtmcEliminationModelChecker.h"



namespace storm {
    namespace modelchecker {
        
        template<typename ParametricType, typename ConstantType>
        class SparseDtmcRegionModelChecker {
        public:
            
           
            
            //The type of variables and bounds depends on the template arguments
            typedef typename std::conditional<(std::is_same<ParametricType,storm::RationalFunction>::value), storm::Variable,std::nullptr_t>::type VariableType;
            typedef typename std::conditional<(std::is_same<ParametricType,storm::RationalFunction>::value), storm::Coefficient,std::nullptr_t>::type CoefficientType;
            
            /*!
             * The possible results for a single region
             */
            enum class RegionCheckResult { 
                UNKNOWN, /*!< the result is unknown */
                EXISTSSAT, /*!< the formula is satisfied for at least one parameter evaluation that lies in the given region */
                EXISTSVIOLATED, /*!< the formula is violated for at least one parameter evaluation that lies in the given region */
                EXISTSBOTH, /*!< the formula is satisfied for some parameters but also violated for others */
                ALLSAT, /*!< the formula is satisfied for all parameters in the given region */
                ALLVIOLATED /*!< the formula is violated for all parameters in the given region */
            };
            
            class ApproximationModel;
            class SamplingModel;
            
            class ParameterRegion{
            public:

                ParameterRegion(std::map<VariableType, CoefficientType> lowerBounds, std::map<VariableType, CoefficientType> upperBounds);
                
                
                
                std::set<VariableType> getVariables() const;
                CoefficientType const& getLowerBound(VariableType const& variable) const;
                CoefficientType const& getUpperBound(VariableType const& variable) const;
                const std::map<VariableType, CoefficientType> getUpperBounds() const;
                const std::map<VariableType, CoefficientType> getLowerBounds() const;
                
                /*
                 * Returns a vector of all possible combinations of lower and upper bounds of the given variables.
                 * The first entry of the returned vector will map every variable to its lower bound
                 * The second entry will map every variable to its lower bound, except the first one (i.e. *consVariables.begin())
                 * ...
                 * The last entry will map every variable to its upper bound
                 * 
                 * If the given set of variables is empty, the returned vector will contain an empty map
                 */
                std::vector<std::map<VariableType, CoefficientType>> getVerticesOfRegion(std::set<VariableType> const& consideredVariables) const;
                
                //returns the currently set check result as a string
                std::string checkResultToString() const;
                
                //returns the region as string in the format 0.3<=p<=0.4,0.2<=q<=0.5;
                std::string toString() const;
                
                void setCheckResult(RegionCheckResult checkResult);
                RegionCheckResult getCheckResult() const;
                
                /*!
                 * Sets a point in the region for which the considered property is not satisfied. 
                 */
                void setViolatedPoint(std::map<VariableType, CoefficientType> const& violatedPoint);
                
                /*!
                 * Retrieves a point in the region for which is considered property is not satisfied.
                 * If such a point is not known, the returned map is empty.
                 */
                std::map<VariableType, CoefficientType> getViolatedPoint() const;
                
                
                /*!
                 * Sets a point in the region for which the considered property is satisfied. 
                 */
                void setSatPoint(std::map<VariableType, CoefficientType> const& satPoint);
                
                /*!
                 * Retrieves a point in the region for which is considered property is satisfied.
                 * If such a point is not known, the returned map is empty.
                 */
                std::map<VariableType, CoefficientType> getSatPoint() const;
                
            private:
                
                std::map<VariableType, CoefficientType> const lowerBounds;
                std::map<VariableType, CoefficientType> const upperBounds;
                std::set<VariableType> variables;
                RegionCheckResult checkResult;
                std::map<VariableType, CoefficientType> satPoint;
                std::map<VariableType, CoefficientType> violatedPoint;
                
                
            };
            
            explicit SparseDtmcRegionModelChecker(storm::models::sparse::Dtmc<ParametricType> const& model);

            /*!
             * Checks if the given formula can be handled by this
             * @param formula the formula to be checked
             */
            bool canHandle(storm::logic::Formula const& formula) const;
            
            /*!
             * Specifies the considered formula.
             * A few preprocessing steps are performed.
             * If another formula has been specified before, all 'context' regarding the old formula is lost.
             * 
             * @param formula the formula to be considered.
             */
            void specifyFormula(storm::logic::Formula const& formula);

            /*!
             * Checks whether the given formula holds for all parameters that lie in the given region.
             * Sets the region checkresult accordingly. Moreover, region.satPoint and/or an region.violatedPoint will be set.
             * 
             * @note A formula has to be specified first.
             * 
             * @param region The considered region
             * 
             */
            void checkRegion(ParameterRegion& region);
            
            /*!
             * Checks for every given region whether the specified formula holds for all parameters that lie in that region.
             * Sets the region checkresult accordingly. Moreover, region.satPoint and/or an region.violatedPoint will be set.
             * 
             * @note A formula has to be specified first.
             * 
             * @param region The considered region
             */
            void checkRegions(std::vector<ParameterRegion>& regions);
            
            /*!
             * Checks whether the given formula holds for all possible parameters that satisfy the given parameter regions
             * ParameterRegions should contain all parameters.
             */
            bool checkRegionOld(storm::logic::Formula const& formula, std::vector<ParameterRegion> parameterRegions);
            
            /*!
             * Prints statistical information (mostly running times) to the given stream.
             */
            void printStatisticsToStream(std::ostream& outstream);
            
            
            
        private:
            
            typedef typename storm::modelchecker::SparseDtmcEliminationModelChecker<ParametricType>::FlexibleSparseMatrix FlexibleMatrix;
            
            /*!
             * Represents the current state of this
             */
           // enum class RegionCheckerState{
         //       NOFORMULA, /*!< there is no formula */
         //       INITIALIZED, /*!< a formula has been specified. Ready to get regions*/
        //    };
            
   #ifdef STORM_HAVE_CARL
                /*!
                 * Instantiates the matrix, i.e., evaluate the occurring functions according to the given substitutions of the variables.
                 * One row of the given flexible matrix will be one rowgroup in the returned matrix, consisting of one row for every substitution.
                 * The returned matrix can be seen as the transition matrix of an MDP with the action labeling given by the returned vector of sets.
                 * Only the rows selected by the given filter are considered. (filter should have size==matrix.getNumberOfRows())
                 * An exception is thrown if there is a transition from a selected state to an unselected state
                 * If one step probabilities are given, a new state is added which can be considered as target state.
                 * The "missing" probability can be redirected to a sink state
                 * By convention, the target state will have index filter.getNumberOfSetBits() and the sink state will be the state with the highest index (so right after the target state)
                 * 
                 * @param matrix the considered flexible matrix
                 * @param substitutions A list of mappings, each assigning a constant value to every variable
                 * @param filter selects the rows of this flexibleMatrix, that will be considered
                 * @param addSinkState adds a state with a self loop to which the "missing" probability will lead
                 * @param oneStepProbabilities if given, a new state is added to which there are transitions for all non-zero entries in this vector
                 * @param addSelfLoops if set, zero valued selfloops will be added in every row
                 * 
                 * @return A matrix with constant (double) entries and a choice labeling
                 */
                std::pair<storm::storage::SparseMatrix<double>,std::vector<boost::container::flat_set<uint_fast64_t>>> instantiateFlexibleMatrix(FlexibleMatrix const& matrix, std::vector<std::map<storm::Variable, storm::RationalFunction::CoeffType>> const& substitutions, storm::storage::BitVector const& filter, bool addSinkState=true, std::vector<ParametricType> const& oneStepProbabilities=std::vector<ParametricType>(), bool addSelfLoops=true) const;
                //todo add const keyword
            
            //eliminates some of the states according to different strategies.
            void eliminateStates(storm::storage::BitVector& subsystem, FlexibleMatrix& flexibleMatrix, std::vector<ParametricType>& oneStepProbabilities, FlexibleMatrix& flexibleBackwardTransitions, storm::storage::BitVector const& initialStates, storm::storage::SparseMatrix<ParametricType> const& forwardTransitions, boost::optional<std::vector<std::size_t>> const& statePriorities = {});
            
            void formulateModelWithSMT(storm::solver::Smt2SmtSolver& solver, std::vector<storm::RationalFunction::PolyType>& stateProbVars, storm::storage::BitVector const& subsystem, FlexibleMatrix const& flexibleMatrix, std::vector<storm::RationalFunction> const& oneStepProbabilities);
            
            void restrictProbabilityVariables(storm::solver::Smt2SmtSolver& solver, std::vector<storm::RationalFunction::PolyType> const& stateProbVars, storm::storage::BitVector const& subsystem, FlexibleMatrix const& flexibleMatrix, std::vector<storm::RationalFunction> const& oneStepProbabilities, ParameterRegion const& region, storm::logic::ComparisonType const& compTypeOfProperty);
#endif         
            template <typename ValueType>
            bool valueIsInBoundOfFormula(ValueType value);
            
            /*!
             * eliminates all states for which the outgoing transitions are constant.
             * Also checks whether the non constant functions are linear
             */
            void computeSimplifiedModel(storm::storage::BitVector const& targetStates);
            
            /*!
             * Initializes a Sample-Model that can be used to get the probability result for a certain parameter evaluation.
             * Initializes an Approximation-Model that can be used to approximate the reachability probabilities.
             */
            void initializeSampleAndApproxModel();
            
            /*!
             * Initializes a DTMC that can be used to get the probability result for a certain parameter evaluation.
             * To quickly insert different evaluations, we provide a dtmc with some dummy values in the transition entries.
             * Furthermore, another matrix is given that is used to connect matrix entries and transition functions.
             * In a similar way an MDP is initialized. The Mdp can be used to approximate the probabilities.
             * In addition to matrix entries and transition functions, we specify which variables and which bounds needs to be substituted
             * 
             * 
             * @param sampleDtmc This dtmc can represent different instantiations of the considered pDtmc
             * @param sampleDtmcMapping Connects the entries of the sampleDtmc Matrix with the corresponding transitionFunctions
             * @param approxMdp This mdp will later be used to approximate the reachability probabilities
             * @param approxMdpMapping Connects the entries of the approxMdp Matrix with the corresponding transitionFunctions and a substitution (given as an index in the approxMdpSubstitutionsVector)
             * @param approxMdpSubstitutions contains the substitutions of the parameters
             * 
             * @param subsys the states of the flexTransitions that are still part of the pDTMC (i.e. that have not been eliminated)
             * @param flexTransitions the transitions of the pDTMC 
             * @param oneStepProbs the probabilities to move to a target state
             * @param initState the initial state of the pDtmc
            void initializeSampleDtmcAndApproxMdp2(
                    std::shared_ptr<storm::models::sparse::Dtmc<ConstantType>> & sampleDtmc,
                    std::vector<std::pair<ParametricType, storm::storage::MatrixEntry<storm::storage::sparse::state_type,ConstantType>&>> & sampleDtmcMapping,
                    std::shared_ptr<storm::models::sparse::Mdp<ConstantType>> & approxMdp,
                    std::vector<std::tuple<ParametricType, storm::storage::MatrixEntry<storm::storage::sparse::state_type,ConstantType>&, std::size_t>> & approxMdpMapping,
                    std::vector<std::map<VariableType, TypeOfBound>> & approxMdpSubstitutions,
                    storm::storage::BitVector const& subsys,
                    storm::storage::SparseMatrix<ParametricType> const& transitions,
                    std::vector<ParametricType> const& oneStepProbs,
                    storm::storage::sparse::state_type const& initState
            );
             */
            
            ParametricType getReachProbFunction();
            
            
            //initializes the given solver which can later be used to give an exact result regarding the whole model.
            void initializeSMTSolver(std::shared_ptr<storm::solver::Smt2SmtSolver>& solver, ParametricType const& reachProbFunction, storm::logic::ProbabilityOperatorFormula const& formula);
            
            /*!
             * Checks the value of the function at some sampling points within the given region.
             * May set the satPoint and violatedPoint of the regions if they are not yet specified and such points are found
             * Also changes the regioncheckresult of the region to EXISTSSAT, EXISTSVIOLATED, or EXISTSBOTH
             * 
             * @return true if an violated point as well as a sat point has been found during the process
             */
            bool checkSamplePoints(ParameterRegion& region);
            
            /*!
             * Checks the value of the function at the given sampling point.
             * May set the satPoint and violatedPoint of the regions if thy are not yet specified and such point is given.
             * Also changes the regioncheckresult of the region to EXISTSSAT, EXISTSVIOLATED, or EXISTSBOTH
             * 
             * @param viaReachProbFunction if set, the sampling will be done via the reachProbFunction.
             *                             Otherwise, the model will be instantiated and checked
             * 
             * @return true if an violated point as well as a sat point has been found, i.e., the check result is changed to EXISTSOTH
             */
            bool checkPoint(ParameterRegion& region, std::map<VariableType, CoefficientType>const& point, bool viaReachProbFunction=false);
            
            /*!
             * Builds an MDP that is used to compute bounds on the maximal/minimal reachability probability,
             * If this approximation already yields that the property is satisfied/violated in the whole region,
             * true is returned and the regioncheckresult of the region is changed accordingly.
             * The computed bounds are stored in the given vectors.
             * However, if only the lowerBounds (or upperBounds) need to be computed in order to prove ALLSAT or ALLVIOLATED, 
             * the upperBounds (or lowerBounds) vector remains untouched.
             */
            bool checkApproximativeProbabilities(ParameterRegion& region, std::vector<ConstantType>& lowerBounds, std::vector<ConstantType>& upperBounds); 
            
            
            /*!
             * Builds the mdp that is used to obtain bounds on the maximal/minimal reachability probability
             * The result is stored in this->approxMdp
             */
            void buildMdpForApproximation(ParameterRegion const& region);
            
            /*!
             * Builds the mdp that is used to obtain bounds on the maximal/minimal reachability probability
             
             */
            storm::models::sparse::Mdp<ConstantType> buildMdpForApproximation2(ParameterRegion const& region);
            
            /*!
             * Starts the SMTSolver to get the result.
             * The current regioncheckresult of the region should be EXISTSSAT or EXISTVIOLATED.
             * Otherwise, a sampingPoint will be computed.
             * True is returned iff the solver was successful (i.e., it returned sat or unsat)
             * A Sat- or Violated point is set, if the solver has found one.
             * The region checkResult of the given region is changed accordingly.
             */
            bool checkFullSmt(ParameterRegion& region); 
            
            
            // The model this model checker is supposed to analyze.
            storm::models::sparse::Dtmc<ParametricType> const& model;

            //classes that provide auxilliary functions
            // Instance of an elimination model checker to access its functions
            storm::modelchecker::SparseDtmcEliminationModelChecker<ParametricType> eliminationModelChecker;
            // comparators that can be used to compare constants.
            storm::utility::ConstantsComparator<ParametricType> parametricTypeComparator;
            storm::utility::ConstantsComparator<ConstantType> constantTypeComparator;
            
            
            std::shared_ptr<storm::solver::Smt2SmtSolver> smtSolver;
            

            //the following members depend on the currently specified formula:
            
            //the currently specified formula
            std::unique_ptr<storm::logic::ProbabilityOperatorFormula> probabilityOperatorFormula;
            
            // the original model after states with constant transitions have been eliminated
            std::shared_ptr<storm::models::sparse::Dtmc<ParametricType>> simplifiedModel;
            
            // a flag that is true if there are only linear functions at transitions of the model
            bool hasOnlyLinearFunctions;
            
            // the model that can be instantiated to check the value at a certain point
            std::shared_ptr<SamplingModel> samplingModel;
            // the model that  is used to approximate the probability values
            std::shared_ptr<ApproximationModel> approximationModel;
            
            /*
            // the dtmc that can be instantiated to check the value at a certain point
            std::shared_ptr<storm::models::sparse::Dtmc<ConstantType>> sampleDtmc;
            // a vector that links entries of the dtmc matrix with the corresponding transition functions (for fast instantiation)
            std::vector<std::pair<ParametricType, storm::storage::MatrixEntry<storm::storage::sparse::state_type,ConstantType>&>> sampleDtmcMapping;
            // the Mdp that is used to approximate the probability values
            std::shared_ptr<storm::models::sparse::Mdp<ConstantType>>  approxMdp;
            // a vector that links entries of the mdp matrix with the corresponding transition functions and substitutions (given as an index of this->approxMdpSubstitutions vector)
            std::vector<std::tuple<ParametricType, storm::storage::MatrixEntry<storm::storage::sparse::state_type,ConstantType>&, std::size_t>> approxMdpMapping;
            // stores the different substitutions of the variables
            std::vector<std::map<VariableType, TypeOfBound>> approxMdpSubstitutions;
              */      
            // The  function for the reachability probability in the initial state 
            ParametricType reachProbFunction;
            bool isReachProbFunctionComputed;
            bool isResultConstant;
            
            
            // runtimes and other information for statistics. 
            uint_fast64_t numOfCheckedRegions;
            
            uint_fast64_t numOfRegionsSolvedThroughSampling;
            uint_fast64_t numOfRegionsSolvedThroughApproximation;
            uint_fast64_t numOfRegionsSolvedThroughSubsystemSmt;
            uint_fast64_t numOfRegionsSolvedThroughFullSmt;
            
            uint_fast64_t numOfRegionsExistsBoth;
            uint_fast64_t numOfRegionsAllSat;
            uint_fast64_t numOfRegionsAllViolated;
            
            std::chrono::high_resolution_clock::duration timePreprocessing;
            std::chrono::high_resolution_clock::duration timeInitialStateElimination;
            std::chrono::high_resolution_clock::duration timeComputeReachProbFunction;
            std::chrono::high_resolution_clock::duration timeCheckRegion;
            std::chrono::high_resolution_clock::duration timeSampling;
            std::chrono::high_resolution_clock::duration timeApproximation;
            std::chrono::high_resolution_clock::duration timeMDPBuild;
            std::chrono::high_resolution_clock::duration timeSubsystemSmt;
            std::chrono::high_resolution_clock::duration timeFullSmt;
        };
        
    } // namespace modelchecker
} // namespace storm

#endif /* STORM_MODELCHECKER_REACHABILITY_SPARSEDTMCREGIONMODELCHECKER_H_ */
