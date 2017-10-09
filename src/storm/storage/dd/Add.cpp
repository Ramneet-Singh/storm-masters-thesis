#include "storm/storage/dd/Add.h"

#include <cstdint>

#include <boost/algorithm/string/join.hpp>

#include "storm/storage/dd/DdMetaVariable.h"
#include "storm/storage/dd/DdManager.h"
#include "storm/storage/dd/Odd.h"

#include "storm/storage/SparseMatrix.h"

#include "storm/utility/constants.h"
#include "storm/utility/macros.h"
#include "storm/exceptions/InvalidArgumentException.h"
#include "storm/exceptions/InvalidOperationException.h"
#include "storm/exceptions/NotSupportedException.h"

#include "storm-config.h"
#include "storm/adapters/RationalFunctionAdapter.h"

namespace storm {
    namespace dd {
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType>::Add(DdManager<LibraryType> const& ddManager, InternalAdd<LibraryType, ValueType> const& internalAdd, std::set<storm::expressions::Variable> const& containedMetaVariables) : Dd<LibraryType>(ddManager, containedMetaVariables), internalAdd(internalAdd) {
            // Intentionally left empty.
        }
        
        template<DdType LibraryType, typename ValueType>
        bool Add<LibraryType, ValueType>::operator==(Add<LibraryType, ValueType> const& other) const {
            return internalAdd == other.internalAdd;
        }

        template<DdType LibraryType, typename ValueType>
        bool Add<LibraryType, ValueType>::operator!=(Add<LibraryType, ValueType> const& other) const {
            return internalAdd != other.internalAdd;
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::operator+(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd + other.internalAdd, Dd<LibraryType>::joinMetaVariables(*this, other));
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType>& Add<LibraryType, ValueType>::operator+=(Add<LibraryType, ValueType> const& other) {
            this->addMetaVariables(other.getContainedMetaVariables());
            internalAdd += other.internalAdd;
            return *this;
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::operator*(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd * other.internalAdd, Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType>& Add<LibraryType, ValueType>::operator*=(Add<LibraryType, ValueType> const& other) {
            this->addMetaVariables(other.getContainedMetaVariables());
            internalAdd *= other.internalAdd;
            return *this;
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::operator-(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd - other.internalAdd, Dd<LibraryType>::joinMetaVariables(*this, other));
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::operator-() const {
            return this->getDdManager().template getAddZero<ValueType>() - *this;
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType>& Add<LibraryType, ValueType>::operator-=(Add<LibraryType, ValueType> const& other) {
            this->addMetaVariables(other.getContainedMetaVariables());
            internalAdd -= other.internalAdd;
            return *this;
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::operator/(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd / other.internalAdd, Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType>& Add<LibraryType, ValueType>::operator/=(Add<LibraryType, ValueType> const& other) {
            this->addMetaVariables(other.getContainedMetaVariables());
            internalAdd /= other.internalAdd;
            return *this;
        }

        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::equals(Add<LibraryType, ValueType> const& other) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.equals(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::notEquals(Add<LibraryType, ValueType> const& other) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.notEquals(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::less(Add<LibraryType, ValueType> const& other) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.less(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::lessOrEqual(Add<LibraryType, ValueType> const& other) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.lessOrEqual(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::greater(Add<LibraryType, ValueType> const& other) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.greater(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }

        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::greaterOrEqual(Add<LibraryType, ValueType> const& other) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.greaterOrEqual(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::pow(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.pow(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::mod(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.mod(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::logxy(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.logxy(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::floor() const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.floor(), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::ceil() const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.ceil(), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, storm::RationalNumber> Add<LibraryType, ValueType>::sharpenKwekMehlhorn(uint64_t precision) const {
            STORM_LOG_THROW(false, storm::exceptions::NotSupportedException, "Operation not supported.");
        }

        template<>
        Add<storm::dd::DdType::Sylvan, storm::RationalNumber> Add<storm::dd::DdType::Sylvan, double>::sharpenKwekMehlhorn(uint64_t precision) const {
            return Add<storm::dd::DdType::Sylvan, storm::RationalNumber>(this->getDdManager(), internalAdd.sharpenKwekMehlhorn(static_cast<std::size_t>(precision)), this->getContainedMetaVariables());
        }

        template<>
        Add<storm::dd::DdType::Sylvan, storm::RationalNumber> Add<storm::dd::DdType::Sylvan, storm::RationalNumber>::sharpenKwekMehlhorn(uint64_t precision) const {
            return Add<storm::dd::DdType::Sylvan, storm::RationalNumber>(this->getDdManager(), internalAdd.sharpenKwekMehlhorn(static_cast<std::size_t>(precision)), this->getContainedMetaVariables());
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::minimum(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.minimum(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::maximum(Add<LibraryType, ValueType> const& other) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.maximum(other), Dd<LibraryType>::joinMetaVariables(*this, other));
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::sumAbstract(std::set<storm::expressions::Variable> const& metaVariables) const {
            Bdd<LibraryType> cube = Bdd<LibraryType>::getCube(this->getDdManager(), metaVariables);
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.sumAbstract(cube.getInternalBdd()), Dd<LibraryType>::subtractMetaVariables(*this, cube));
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::minAbstract(std::set<storm::expressions::Variable> const& metaVariables) const {
            Bdd<LibraryType> cube = Bdd<LibraryType>::getCube(this->getDdManager(), metaVariables);
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.minAbstract(cube.getInternalBdd()), Dd<LibraryType>::subtractMetaVariables(*this, cube));
        }
		
		template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::minAbstractRepresentative(std::set<storm::expressions::Variable> const& metaVariables) const {
            Bdd<LibraryType> cube = Bdd<LibraryType>::getCube(this->getDdManager(), metaVariables);
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.minAbstractRepresentative(cube.getInternalBdd()), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::maxAbstract(std::set<storm::expressions::Variable> const& metaVariables) const {
            Bdd<LibraryType> cube = Bdd<LibraryType>::getCube(this->getDdManager(), metaVariables);
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.maxAbstract(cube.getInternalBdd()), Dd<LibraryType>::subtractMetaVariables(*this, cube));
        }
		
		template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::maxAbstractRepresentative(std::set<storm::expressions::Variable> const& metaVariables) const {
            Bdd<LibraryType> cube = Bdd<LibraryType>::getCube(this->getDdManager(), metaVariables);
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.maxAbstractRepresentative(cube.getInternalBdd()), this->getContainedMetaVariables());
        }

        template<DdType LibraryType, typename ValueType>
        bool Add<LibraryType, ValueType>::equalModuloPrecision(Add<LibraryType, ValueType> const& other, ValueType const& precision, bool relative) const {
            return internalAdd.equalModuloPrecision(other, precision, relative);
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::renameVariables(std::set<storm::expressions::Variable> const& from, std::set<storm::expressions::Variable> const& to) const {
            std::vector<InternalBdd<LibraryType>> fromBdds;
            std::vector<InternalBdd<LibraryType>> toBdds;
            
            for (auto const& metaVariable : from) {
                STORM_LOG_THROW(this->containsMetaVariable(metaVariable), storm::exceptions::InvalidOperationException, "Cannot rename variable '" << metaVariable.getName() << "' that is not present.");
                DdMetaVariable<LibraryType> const& ddMetaVariable = this->getDdManager().getMetaVariable(metaVariable);
                for (auto const& ddVariable : ddMetaVariable.getDdVariables()) {
                    fromBdds.push_back(ddVariable.getInternalBdd());
                }
            }
            for (auto const& metaVariable : to) {
                STORM_LOG_THROW(!this->containsMetaVariable(metaVariable), storm::exceptions::InvalidOperationException, "Cannot rename to variable '" << metaVariable.getName() << "' that is already present.");
                DdMetaVariable<LibraryType> const& ddMetaVariable = this->getDdManager().getMetaVariable(metaVariable);
                for (auto const& ddVariable : ddMetaVariable.getDdVariables()) {
                    toBdds.push_back(ddVariable.getInternalBdd());
                }
            }
            
            std::set<storm::expressions::Variable> newContainedMetaVariables = to;
            std::set_difference(this->getContainedMetaVariables().begin(), this->getContainedMetaVariables().end(), from.begin(), from.end(), std::inserter(newContainedMetaVariables, newContainedMetaVariables.begin()));
            
            STORM_LOG_THROW(fromBdds.size() == toBdds.size(), storm::exceptions::InvalidArgumentException, "Unable to rename mismatching meta variables.");
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.swapVariables(fromBdds, toBdds), newContainedMetaVariables);
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::swapVariables(std::vector<std::pair<storm::expressions::Variable, storm::expressions::Variable>> const& metaVariablePairs) const {
            std::set<storm::expressions::Variable> newContainedMetaVariables;
            std::set<storm::expressions::Variable> deletedMetaVariables;
            std::vector<InternalBdd<LibraryType>> from;
            std::vector<InternalBdd<LibraryType>> to;
            for (auto const& metaVariablePair : metaVariablePairs) {
                DdMetaVariable<LibraryType> const& variable1 = this->getDdManager().getMetaVariable(metaVariablePair.first);
                DdMetaVariable<LibraryType> const& variable2 = this->getDdManager().getMetaVariable(metaVariablePair.second);
                
                // Keep track of the contained meta variables in the DD.
                if (this->containsMetaVariable(metaVariablePair.first)) {
                    if (this->containsMetaVariable(metaVariablePair.second)) {
                        // Nothing to do here.
                    } else {
                        newContainedMetaVariables.insert(metaVariablePair.second);
                        deletedMetaVariables.insert(metaVariablePair.first);
                    }
                } else {
                    if (!this->containsMetaVariable(metaVariablePair.second)) {
                        // Nothing to do here.
                    } else {
                        newContainedMetaVariables.insert(metaVariablePair.first);
                        deletedMetaVariables.insert(metaVariablePair.second);
                    }
                }
                for (auto const& ddVariable : variable1.getDdVariables()) {
                    from.push_back(ddVariable.getInternalBdd());
                }
                for (auto const& ddVariable : variable2.getDdVariables()) {
                    to.push_back(ddVariable.getInternalBdd());
                }
            }
            
            std::set<storm::expressions::Variable> tmp;
            std::set_difference(this->getContainedMetaVariables().begin(), this->getContainedMetaVariables().end(), deletedMetaVariables.begin(), deletedMetaVariables.end(), std::inserter(tmp, tmp.begin()));
            std::set<storm::expressions::Variable> containedMetaVariables;
            std::set_union(tmp.begin(), tmp.end(), newContainedMetaVariables.begin(), newContainedMetaVariables.end(), std::inserter(containedMetaVariables, containedMetaVariables.begin()));
            STORM_LOG_THROW(from.size() == to.size(), storm::exceptions::InvalidArgumentException, "Unable to swap mismatching meta variables.");
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.swapVariables(from, to), containedMetaVariables);
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::permuteVariables(std::vector<std::pair<storm::expressions::Variable, storm::expressions::Variable>> const& metaVariablePairs) const {
            std::set<storm::expressions::Variable> newContainedMetaVariables;
            std::set<storm::expressions::Variable> deletedMetaVariables;
            std::vector<InternalBdd<LibraryType>> from;
            std::vector<InternalBdd<LibraryType>> to;
            for (auto const& metaVariablePair : metaVariablePairs) {
                DdMetaVariable<LibraryType> const& variable1 = this->getDdManager().getMetaVariable(metaVariablePair.first);
                DdMetaVariable<LibraryType> const& variable2 = this->getDdManager().getMetaVariable(metaVariablePair.second);
                
                // Keep track of the contained meta variables in the DD.
                if (this->containsMetaVariable(metaVariablePair.first)) {
                    deletedMetaVariables.insert(metaVariablePair.first);
                    newContainedMetaVariables.insert(metaVariablePair.second);
                }
                
                for (auto const& ddVariable : variable1.getDdVariables()) {
                    from.push_back(ddVariable.getInternalBdd());
                }
                for (auto const& ddVariable : variable2.getDdVariables()) {
                    to.push_back(ddVariable.getInternalBdd());
                }
            }
            
            std::set<storm::expressions::Variable> tmp;
            std::set_difference(this->getContainedMetaVariables().begin(), this->getContainedMetaVariables().end(), deletedMetaVariables.begin(), deletedMetaVariables.end(), std::inserter(tmp, tmp.begin()));
            std::set<storm::expressions::Variable> containedMetaVariables;
            std::set_union(tmp.begin(), tmp.end(), newContainedMetaVariables.begin(), newContainedMetaVariables.end(), std::inserter(containedMetaVariables, containedMetaVariables.begin()));
            STORM_LOG_THROW(from.size() == to.size(), storm::exceptions::InvalidArgumentException, "Unable to swap mismatching meta variables.");
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.permuteVariables(from, to), containedMetaVariables);
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::multiplyMatrix(Add<LibraryType, ValueType> const& otherMatrix, std::set<storm::expressions::Variable> const& summationMetaVariables) const {
            // Create the summation variables.
            std::vector<InternalBdd<LibraryType>> summationDdVariables;
            for (auto const& metaVariable : summationMetaVariables) {
                for (auto const& ddVariable : this->getDdManager().getMetaVariable(metaVariable).getDdVariables()) {
                    summationDdVariables.push_back(ddVariable.getInternalBdd());
                }
            }
            
            std::set<storm::expressions::Variable> unionOfMetaVariables = Dd<LibraryType>::joinMetaVariables(*this, otherMatrix);
            std::set<storm::expressions::Variable> containedMetaVariables;
            std::set_difference(unionOfMetaVariables.begin(), unionOfMetaVariables.end(), summationMetaVariables.begin(), summationMetaVariables.end(), std::inserter(containedMetaVariables, containedMetaVariables.begin()));
            
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.multiplyMatrix(otherMatrix, summationDdVariables), containedMetaVariables);
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::multiplyMatrix(Bdd<LibraryType> const& otherMatrix, std::set<storm::expressions::Variable> const& summationMetaVariables) const {
            // Create the summation variables.
            std::vector<InternalBdd<LibraryType>> summationDdVariables;
            for (auto const& metaVariable : summationMetaVariables) {
                for (auto const& ddVariable : this->getDdManager().getMetaVariable(metaVariable).getDdVariables()) {
                    summationDdVariables.push_back(ddVariable.getInternalBdd());
                }
            }
            
            std::set<storm::expressions::Variable> unionOfMetaVariables = Dd<LibraryType>::joinMetaVariables(*this, otherMatrix);
            std::set<storm::expressions::Variable> containedMetaVariables;
            std::set_difference(unionOfMetaVariables.begin(), unionOfMetaVariables.end(), summationMetaVariables.begin(), summationMetaVariables.end(), std::inserter(containedMetaVariables, containedMetaVariables.begin()));
            
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.multiplyMatrix(otherMatrix.getInternalBdd(), summationDdVariables), containedMetaVariables);
        }

        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::greater(ValueType const& value) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.greater(value), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::greaterOrEqual(ValueType const& value) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.greaterOrEqual(value), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::less(ValueType const& value) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.less(value), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::lessOrEqual(ValueType const& value) const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.lessOrEqual(value), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::notZero() const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.notZero(), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::constrain(Add<LibraryType, ValueType> const& constraint) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.constrain(constraint), Dd<LibraryType>::joinMetaVariables(*this, constraint));
        }
        
        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::restrict(Add<LibraryType, ValueType> const& constraint) const {
            return Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.restrict(constraint), Dd<LibraryType>::joinMetaVariables(*this, constraint));
        }

        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::getSupport() const {
            return Bdd<LibraryType>(this->getDdManager(), internalAdd.getSupport(), this->getContainedMetaVariables());
        }
        
        template<DdType LibraryType, typename ValueType>
        uint_fast64_t Add<LibraryType, ValueType>::getNonZeroCount() const {
            std::size_t numberOfDdVariables = 0;
            for (auto const& metaVariable : this->getContainedMetaVariables()) {
                numberOfDdVariables += this->getDdManager().getMetaVariable(metaVariable).getNumberOfDdVariables();
            }
            return internalAdd.getNonZeroCount(numberOfDdVariables);
        }
        
        template<DdType LibraryType, typename ValueType>
        uint_fast64_t Add<LibraryType, ValueType>::getLeafCount() const {
            return internalAdd.getLeafCount();
        }
        
        template<DdType LibraryType, typename ValueType>
        uint_fast64_t Add<LibraryType, ValueType>::getNodeCount() const {
            return internalAdd.getNodeCount();
        }
        
        template<DdType LibraryType, typename ValueType>
        ValueType Add<LibraryType, ValueType>::getMin() const {
            return internalAdd.getMin();
        }
        
        template<DdType LibraryType, typename ValueType>
        ValueType Add<LibraryType, ValueType>::getMax() const {
            return internalAdd.getMax();
        }

        template<DdType LibraryType, typename ValueType>
        void Add<LibraryType, ValueType>::setValue(storm::expressions::Variable const& metaVariable, int_fast64_t variableValue, ValueType const& targetValue) {
            std::map<storm::expressions::Variable, int_fast64_t> metaVariableToValueMap;
            metaVariableToValueMap.emplace(metaVariable, variableValue);
            this->setValue(metaVariableToValueMap, targetValue);
        }
        
        template<DdType LibraryType, typename ValueType>
        void Add<LibraryType, ValueType>::setValue(storm::expressions::Variable const& metaVariable1, int_fast64_t variableValue1, storm::expressions::Variable const& metaVariable2, int_fast64_t variableValue2, ValueType const& targetValue) {
            std::map<storm::expressions::Variable, int_fast64_t> metaVariableToValueMap;
            metaVariableToValueMap.emplace(metaVariable1, variableValue1);
            metaVariableToValueMap.emplace(metaVariable2, variableValue2);
            this->setValue(metaVariableToValueMap, targetValue);
        }

        template<DdType LibraryType, typename ValueType>
        void Add<LibraryType, ValueType>::setValue(std::map<storm::expressions::Variable, int_fast64_t> const& metaVariableToValueMap, ValueType const& targetValue) {
            Bdd<LibraryType> valueEncoding = this->getDdManager().getBddOne();
            for (auto const& nameValuePair : metaVariableToValueMap) {
                valueEncoding &= this->getDdManager().getEncoding(nameValuePair.first, nameValuePair.second);
                // Also record that the DD now contains the meta variable.
                this->addMetaVariable(nameValuePair.first);
            }
            
            internalAdd = valueEncoding.ite(this->getDdManager().getConstant(targetValue), *this);
        }

        template<DdType LibraryType, typename ValueType>
        ValueType Add<LibraryType, ValueType>::getValue(std::map<storm::expressions::Variable, int_fast64_t> const& metaVariableToValueMap) const {
            std::set<storm::expressions::Variable> remainingMetaVariables(this->getContainedMetaVariables());
            Bdd<LibraryType> valueEncoding = this->getDdManager().getBddOne();
            for (auto const& nameValuePair : metaVariableToValueMap) {
                valueEncoding &= this->getDdManager().getEncoding(nameValuePair.first, nameValuePair.second);
                if (this->containsMetaVariable(nameValuePair.first)) {
                    remainingMetaVariables.erase(nameValuePair.first);
                }
            }
            
            STORM_LOG_THROW(remainingMetaVariables.empty(), storm::exceptions::InvalidArgumentException, "Cannot evaluate function for which not all inputs were given.");
            
            Add<LibraryType, ValueType> value = *this * valueEncoding.template toAdd<ValueType>();
            value = value.sumAbstract(this->getContainedMetaVariables());
            return value.internalAdd.getValue();
        }
        
        template<DdType LibraryType, typename ValueType>
        bool Add<LibraryType, ValueType>::isOne() const {
            return internalAdd.isOne();
        }
        
        template<DdType LibraryType, typename ValueType>
        bool Add<LibraryType, ValueType>::isZero() const {
            return internalAdd.isZero();
        }
        
        template<DdType LibraryType, typename ValueType>
        bool Add<LibraryType, ValueType>::isConstant() const {
            return internalAdd.isConstant();
        }
        
        template<DdType LibraryType, typename ValueType>
        uint_fast64_t Add<LibraryType, ValueType>::getIndex() const {
            return internalAdd.getIndex();
        }
        
        template<DdType LibraryType, typename ValueType>
        uint_fast64_t Add<LibraryType, ValueType>::getLevel() const {
            return internalAdd.getLevel();
        }
        
        template<DdType LibraryType, typename ValueType>
        std::vector<ValueType> Add<LibraryType, ValueType>::toVector() const {
            return this->toVector(this->createOdd());
        }
        
        template<DdType LibraryType, typename ValueType>
        std::vector<ValueType> Add<LibraryType, ValueType>::toVector(Odd const& rowOdd) const {
            std::vector<ValueType> result(rowOdd.getTotalOffset());
            std::vector<uint_fast64_t> ddVariableIndices = this->getSortedVariableIndices();
            internalAdd.composeWithExplicitVector(rowOdd, ddVariableIndices, result, std::plus<ValueType>());
            return result;
        }
        
        template<DdType LibraryType, typename ValueType>
        std::vector<ValueType> Add<LibraryType, ValueType>::toVector(storm::dd::Add<LibraryType, ValueType> const& matrix, std::vector<uint_fast64_t> const& rowGroupIndices, std::set<storm::expressions::Variable> const& rowMetaVariables, std::set<storm::expressions::Variable> const& columnMetaVariables, std::set<storm::expressions::Variable> const& groupMetaVariables, storm::dd::Odd const& rowOdd, storm::dd::Odd const& columnOdd) const {
            std::vector<uint_fast64_t> ddRowVariableIndices;
            std::vector<uint_fast64_t> ddColumnVariableIndices;
            std::vector<uint_fast64_t> ddGroupVariableIndices;
            std::set<storm::expressions::Variable> rowAndColumnMetaVariables;
            
            for (auto const& variable : rowMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddRowVariableIndices.push_back(ddVariable.getIndex());
                }
                rowAndColumnMetaVariables.insert(variable);
            }
            std::sort(ddRowVariableIndices.begin(), ddRowVariableIndices.end());
            for (auto const& variable : columnMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddColumnVariableIndices.push_back(ddVariable.getIndex());
                }
                rowAndColumnMetaVariables.insert(variable);
            }
            std::sort(ddColumnVariableIndices.begin(), ddColumnVariableIndices.end());
            for (auto const& variable : groupMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddGroupVariableIndices.push_back(ddVariable.getIndex());
                }
            }
            std::sort(ddGroupVariableIndices.begin(), ddGroupVariableIndices.end());
            
            Bdd<LibraryType> columnVariableCube = Bdd<LibraryType>::getCube(this->getDdManager(), columnMetaVariables);
            
            // Copy the row group indices so we can modify them.
            std::vector<uint_fast64_t> mutableRowGroupIndices = rowGroupIndices;
            
            // Create the explicit vector we need to fill later.
            std::vector<ValueType> explicitVector(mutableRowGroupIndices.back());
            
            // Next, we split the matrix into one for each group. Note that this only works if the group variables are at the very top.
            std::vector<std::pair<InternalAdd<LibraryType, ValueType>, InternalAdd<LibraryType, ValueType>>> internalAddGroups = matrix.internalAdd.splitIntoGroups(*this, ddGroupVariableIndices);
            std::vector<std::pair<Add<LibraryType, ValueType>, Add<LibraryType, ValueType>>> groups;
            for (auto const& internalAdd : internalAddGroups) {
                groups.push_back(std::make_pair(Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.first, rowAndColumnMetaVariables), Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.second, rowMetaVariables)));
            }
            
            std::vector<InternalAdd<LibraryType, uint_fast64_t>> statesWithGroupEnabled(groups.size());
            for (uint_fast64_t i = 0; i < groups.size(); ++i) {
                std::pair<Add<LibraryType, ValueType>, Add<LibraryType, ValueType>> const& ddPair = groups[i];
                Bdd<LibraryType> matrixDdNotZero = ddPair.first.notZero();
                Bdd<LibraryType> vectorDdNotZero = ddPair.second.notZero();
                
                ddPair.second.internalAdd.composeWithExplicitVector(rowOdd, ddRowVariableIndices, mutableRowGroupIndices, explicitVector, std::plus<ValueType>());
                
                InternalAdd<LibraryType, uint_fast64_t> statesWithGroupEnabled = (matrixDdNotZero.existsAbstract(columnMetaVariables) || vectorDdNotZero).template toAdd<uint_fast64_t>();
                statesWithGroupEnabled.composeWithExplicitVector(rowOdd, ddRowVariableIndices, mutableRowGroupIndices, std::plus<uint_fast64_t>());
            }
            
            return explicitVector;
        }
                
        template<DdType LibraryType, typename ValueType>
        storm::storage::SparseMatrix<ValueType> Add<LibraryType, ValueType>::toMatrix() const {
            std::set<storm::expressions::Variable> rowVariables;
            std::set<storm::expressions::Variable> columnVariables;
            
            for (auto const& variable : this->getContainedMetaVariables()) {
                if (variable.getName().size() > 0 && variable.getName().back() == '\'') {
                    columnVariables.insert(variable);
                } else {
                    rowVariables.insert(variable);
                }
            }
            
            return toMatrix(rowVariables, columnVariables, this->sumAbstract(rowVariables).createOdd(), this->sumAbstract(columnVariables).createOdd());
        }
        
        template<DdType LibraryType, typename ValueType>
        storm::storage::SparseMatrix<ValueType> Add<LibraryType, ValueType>::toMatrix(storm::dd::Odd const& rowOdd, storm::dd::Odd const& columnOdd) const {
            std::set<storm::expressions::Variable> rowMetaVariables;
            std::set<storm::expressions::Variable> columnMetaVariables;
            
            for (auto const& variable : this->getContainedMetaVariables()) {
                if (variable.getName().size() > 0 && variable.getName().back() == '\'') {
                    columnMetaVariables.insert(variable);
                } else {
                    rowMetaVariables.insert(variable);
                }
            }
            
            return toMatrix(rowMetaVariables, columnMetaVariables, rowOdd, columnOdd);
        }
        
        template<DdType LibraryType, typename ValueType>
        storm::storage::SparseMatrix<ValueType> Add<LibraryType, ValueType>::toMatrix(std::set<storm::expressions::Variable> const& rowMetaVariables, std::set<storm::expressions::Variable> const& columnMetaVariables, storm::dd::Odd const& rowOdd, storm::dd::Odd const& columnOdd) const {
            std::vector<uint_fast64_t> ddRowVariableIndices;
            std::vector<uint_fast64_t> ddColumnVariableIndices;
            
            for (auto const& variable : rowMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddRowVariableIndices.push_back(ddVariable.getIndex());
                }
            }
            std::sort(ddRowVariableIndices.begin(), ddRowVariableIndices.end());
            
            for (auto const& variable : columnMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddColumnVariableIndices.push_back(ddVariable.getIndex());
                }
            }
            std::sort(ddColumnVariableIndices.begin(), ddColumnVariableIndices.end());
            
            // Prepare the vectors that represent the matrix.
            std::vector<uint_fast64_t> rowIndications(rowOdd.getTotalOffset() + 1);
            std::vector<storm::storage::MatrixEntry<uint_fast64_t, ValueType>> columnsAndValues(this->getNonZeroCount());
            
            // Create a trivial row grouping.
            std::vector<uint_fast64_t> trivialRowGroupIndices(rowIndications.size());
            uint_fast64_t i = 0;
            for (auto& entry : trivialRowGroupIndices) {
                entry = i;
                ++i;
            }
            
            // Count the number of elements in the rows.
            rowIndications = this->notZero().template toAdd<uint_fast64_t>().sumAbstract(columnMetaVariables).toVector(rowOdd);
            rowIndications.emplace_back();
            
            // Now that we computed the number of entries in each row, compute the corresponding offsets in the entry vector.
            uint_fast64_t tmp = 0;
            uint_fast64_t tmp2 = 0;
            for (uint_fast64_t i = 1; i < rowIndications.size(); ++i) {
                tmp2 = rowIndications[i];
                rowIndications[i] = rowIndications[i - 1] + tmp;
                std::swap(tmp, tmp2);
            }
            rowIndications[0] = 0;
            
            // Now actually fill the entry vector.
            internalAdd.toMatrixComponents(trivialRowGroupIndices, rowIndications, columnsAndValues, rowOdd, columnOdd, ddRowVariableIndices, ddColumnVariableIndices, true);
            
            // Since the last call to toMatrixRec modified the rowIndications, we need to restore the correct values.
            for (uint_fast64_t i = rowIndications.size() - 1; i > 0; --i) {
                rowIndications[i] = rowIndications[i - 1];
            }
            rowIndications[0] = 0;
            
            // Construct matrix and return result.
            return storm::storage::SparseMatrix<ValueType>(columnOdd.getTotalOffset(), std::move(rowIndications), std::move(columnsAndValues), boost::none);
        }
        
        template<DdType LibraryType, typename ValueType>
        storm::storage::SparseMatrix<ValueType> Add<LibraryType, ValueType>::toMatrix(std::set<storm::expressions::Variable> const& groupMetaVariables, storm::dd::Odd const& rowOdd, storm::dd::Odd const& columnOdd) const {
            std::set<storm::expressions::Variable> rowMetaVariables;
            std::set<storm::expressions::Variable> columnMetaVariables;
            
            for (auto const& variable : this->getContainedMetaVariables()) {
                // If the meta variable is a group meta variable, we do not insert it into the set of row/column meta variables.
                if (groupMetaVariables.find(variable) != groupMetaVariables.end()) {
                    continue;
                }
                
                if (variable.getName().size() > 0 && variable.getName().back() == '\'') {
                    columnMetaVariables.insert(variable);
                } else {
                    rowMetaVariables.insert(variable);
                }
            }
            
            // Create the canonical row group sizes and build the matrix.
            return toMatrix(rowMetaVariables, columnMetaVariables, groupMetaVariables, rowOdd, columnOdd);
        }

        template<DdType LibraryType, typename ValueType>
        storm::storage::SparseMatrix<ValueType> Add<LibraryType, ValueType>::toMatrix(std::set<storm::expressions::Variable> const& rowMetaVariables, std::set<storm::expressions::Variable> const& columnMetaVariables, std::set<storm::expressions::Variable> const& groupMetaVariables, storm::dd::Odd const& rowOdd, storm::dd::Odd const& columnOdd) const {
            std::vector<uint_fast64_t> ddRowVariableIndices;
            std::vector<uint_fast64_t> ddColumnVariableIndices;
            std::vector<uint_fast64_t> ddGroupVariableIndices;
            std::set<storm::expressions::Variable> rowAndColumnMetaVariables;
            
            for (auto const& variable : rowMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddRowVariableIndices.push_back(ddVariable.getIndex());
                }
                rowAndColumnMetaVariables.insert(variable);
            }
            std::sort(ddRowVariableIndices.begin(), ddRowVariableIndices.end());
            for (auto const& variable : columnMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddColumnVariableIndices.push_back(ddVariable.getIndex());
                }
                rowAndColumnMetaVariables.insert(variable);
            }
            std::sort(ddColumnVariableIndices.begin(), ddColumnVariableIndices.end());
            for (auto const& variable : groupMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddGroupVariableIndices.push_back(ddVariable.getIndex());
                }
            }
            std::sort(ddGroupVariableIndices.begin(), ddGroupVariableIndices.end());
            
            Bdd<LibraryType> columnVariableCube = Bdd<LibraryType>::getCube(this->getDdManager(), columnMetaVariables);
            
            // Start by computing the offsets (in terms of rows) for each row group.
            Add<LibraryType, uint_fast64_t> stateToNumberOfChoices = this->notZero().existsAbstract(columnMetaVariables).template toAdd<uint_fast64_t>().sumAbstract(groupMetaVariables);
            std::vector<uint_fast64_t> rowGroupIndices = stateToNumberOfChoices.toVector(rowOdd);
            rowGroupIndices.resize(rowGroupIndices.size() + 1);
            uint_fast64_t tmp = 0;
            uint_fast64_t tmp2 = 0;
            for (uint_fast64_t i = 1; i < rowGroupIndices.size(); ++i) {
                tmp2 = rowGroupIndices[i];
                rowGroupIndices[i] = rowGroupIndices[i - 1] + tmp;
                std::swap(tmp, tmp2);
            }
            rowGroupIndices[0] = 0;
            
            // Next, we split the matrix into one for each group. Note that this only works if the group variables are
            // at the very top.
            std::vector<InternalAdd<LibraryType, ValueType>> internalAddGroups = internalAdd.splitIntoGroups(ddGroupVariableIndices);
            std::vector<Add<LibraryType, ValueType>> groups;
            for (auto const& internalAdd : internalAddGroups) {
                groups.push_back(Add<LibraryType, ValueType>(this->getDdManager(), internalAdd, rowAndColumnMetaVariables));
            }
            
            // Create the actual storage for the non-zero entries.
            std::vector<storm::storage::MatrixEntry<uint_fast64_t, ValueType>> columnsAndValues(this->getNonZeroCount());
            
            // Now compute the indices at which the individual rows start.
            std::vector<uint_fast64_t> rowIndications(rowGroupIndices.back() + 1);
            
            std::vector<InternalAdd<LibraryType, uint_fast64_t>> statesWithGroupEnabled(groups.size());
            InternalAdd<LibraryType, uint_fast64_t> stateToRowGroupCount = this->getDdManager().template getAddZero<uint_fast64_t>();
            for (uint_fast64_t i = 0; i < groups.size(); ++i) {
                auto const& group = groups[i];
                auto groupNotZero = group.notZero();
                
                std::vector<uint64_t> tmpRowIndications = groupNotZero.template toAdd<uint_fast64_t>().sumAbstract(columnMetaVariables).toVector(rowOdd);
                for (uint64_t offset = 0; offset < tmpRowIndications.size(); ++offset) {
                    rowIndications[rowGroupIndices[offset]] += tmpRowIndications[offset];
                }
                
                statesWithGroupEnabled[i] = groupNotZero.existsAbstract(columnMetaVariables).template toAdd<uint_fast64_t>();
                statesWithGroupEnabled[i].composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::plus<uint_fast64_t>());
            }
            
            // Since we modified the rowGroupIndices, we need to restore the correct values.
            stateToNumberOfChoices.internalAdd.composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::minus<uint_fast64_t>());
            
            // Now that we computed the number of entries in each row, compute the corresponding offsets in the entry vector.
            tmp = 0;
            tmp2 = 0;
            for (uint_fast64_t i = 1; i < rowIndications.size(); ++i) {
                tmp2 = rowIndications[i];
                rowIndications[i] = rowIndications[i - 1] + tmp;
                std::swap(tmp, tmp2);
            }
            rowIndications[0] = 0;
            
            // Now actually fill the entry vector.
            for (uint_fast64_t i = 0; i < groups.size(); ++i) {
                auto const& group = groups[i];
                
                group.internalAdd.toMatrixComponents(rowGroupIndices, rowIndications, columnsAndValues, rowOdd, columnOdd, ddRowVariableIndices, ddColumnVariableIndices, true);
                
                statesWithGroupEnabled[i].composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::plus<uint_fast64_t>());
            }
            
            // Since we modified the rowGroupIndices, we need to restore the correct values.
            stateToNumberOfChoices.internalAdd.composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::minus<uint_fast64_t>());
            
            // Since the last call to toMatrixRec modified the rowIndications, we need to restore the correct values.
            for (uint_fast64_t i = rowIndications.size() - 1; i > 0; --i) {
                rowIndications[i] = rowIndications[i - 1];
            }
            rowIndications[0] = 0;
            
            return storm::storage::SparseMatrix<ValueType>(columnOdd.getTotalOffset(), std::move(rowIndications), std::move(columnsAndValues), std::move(rowGroupIndices));
        }

        template<DdType LibraryType, typename ValueType>
        std::pair<storm::storage::SparseMatrix<ValueType>, std::vector<ValueType>> Add<LibraryType, ValueType>::toMatrixVector(storm::dd::Add<LibraryType, ValueType> const& vector, std::set<storm::expressions::Variable> const& groupMetaVariables, storm::dd::Odd const& rowOdd, storm::dd::Odd const& columnOdd) const {
            std::set<storm::expressions::Variable> rowMetaVariables;
            std::set<storm::expressions::Variable> columnMetaVariables;
            
            for (auto const& variable : this->getContainedMetaVariables()) {
                // If the meta variable is a group meta variable, we do not insert it into the set of row/column meta variables.
                if (groupMetaVariables.find(variable) != groupMetaVariables.end()) {
                    continue;
                }
                
                if (variable.getName().size() > 0 && variable.getName().back() == '\'') {
                    columnMetaVariables.insert(variable);
                } else {
                    rowMetaVariables.insert(variable);
                }
            }
            
            // Create the canonical row group sizes and build the matrix.
            return toMatrixVector(vector, rowMetaVariables, columnMetaVariables, groupMetaVariables, rowOdd, columnOdd);
        }

        template<DdType LibraryType, typename ValueType>
        std::pair<storm::storage::SparseMatrix<ValueType>, std::vector<ValueType>> Add<LibraryType, ValueType>::toMatrixVector(storm::dd::Add<LibraryType, ValueType> const& vector, std::set<storm::expressions::Variable> const& rowMetaVariables, std::set<storm::expressions::Variable> const& columnMetaVariables, std::set<storm::expressions::Variable> const& groupMetaVariables, storm::dd::Odd const& rowOdd, storm::dd::Odd const& columnOdd) const {
            std::vector<uint_fast64_t> ddRowVariableIndices;
            std::vector<uint_fast64_t> ddColumnVariableIndices;
            std::vector<uint_fast64_t> ddGroupVariableIndices;
            std::set<storm::expressions::Variable> rowAndColumnMetaVariables;
            
            for (auto const& variable : rowMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddRowVariableIndices.push_back(ddVariable.getIndex());
                }
                rowAndColumnMetaVariables.insert(variable);
            }
            std::sort(ddRowVariableIndices.begin(), ddRowVariableIndices.end());
            for (auto const& variable : columnMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddColumnVariableIndices.push_back(ddVariable.getIndex());
                }
                rowAndColumnMetaVariables.insert(variable);
            }
            std::sort(ddColumnVariableIndices.begin(), ddColumnVariableIndices.end());
            for (auto const& variable : groupMetaVariables) {
                DdMetaVariable<LibraryType> const& metaVariable = this->getDdManager().getMetaVariable(variable);
                for (auto const& ddVariable : metaVariable.getDdVariables()) {
                    ddGroupVariableIndices.push_back(ddVariable.getIndex());
                }
            }
            std::sort(ddGroupVariableIndices.begin(), ddGroupVariableIndices.end());
            
            Bdd<LibraryType> columnVariableCube = Bdd<LibraryType>::getCube(this->getDdManager(), columnMetaVariables);

            // Count how many choices each row group has.
            std::vector<uint_fast64_t> rowGroupIndices = (this->notZero().existsAbstract(columnMetaVariables) || vector.notZero()).template toAdd<uint_fast64_t>().sumAbstract(groupMetaVariables).toVector(rowOdd);
            
            // Transform the row group sizes to the actual row group indices.
            rowGroupIndices.resize(rowGroupIndices.size() + 1);
            uint_fast64_t tmp = 0;
            uint_fast64_t tmp2 = 0;
            for (uint_fast64_t i = 1; i < rowGroupIndices.size(); ++i) {
                tmp2 = rowGroupIndices[i];
                rowGroupIndices[i] = rowGroupIndices[i - 1] + tmp;
                std::swap(tmp, tmp2);
            }
            rowGroupIndices[0] = 0;
            
            // Create the explicit vector we need to fill later.
            std::vector<ValueType> explicitVector(rowGroupIndices.back());
            
            // Next, we split the matrix into one for each group. Note that this only works if the group variables are at the very top.
            std::vector<std::pair<InternalAdd<LibraryType, ValueType>, InternalAdd<LibraryType, ValueType>>> internalAddGroups = internalAdd.splitIntoGroups(vector, ddGroupVariableIndices);
            std::vector<std::pair<Add<LibraryType, ValueType>, Add<LibraryType, ValueType>>> groups;
            for (auto const& internalAdd : internalAddGroups) {
                groups.push_back(std::make_pair(Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.first, rowAndColumnMetaVariables), Add<LibraryType, ValueType>(this->getDdManager(), internalAdd.second, rowMetaVariables)));
            }

            // Create the actual storage for the non-zero entries.
            std::vector<storm::storage::MatrixEntry<uint_fast64_t, ValueType>> columnsAndValues(this->getNonZeroCount());
            
            // Now compute the indices at which the individual rows start.
            std::vector<uint_fast64_t> rowIndications(rowGroupIndices.back() + 1);
            
            std::vector<InternalAdd<LibraryType, uint_fast64_t>> statesWithGroupEnabled(groups.size());
            InternalAdd<LibraryType, uint_fast64_t> stateToRowGroupCount = this->getDdManager().template getAddZero<uint_fast64_t>();
            for (uint_fast64_t i = 0; i < groups.size(); ++i) {
                std::pair<Add<LibraryType, ValueType>, Add<LibraryType, ValueType>> const& ddPair = groups[i];
                Bdd<LibraryType> matrixDdNotZero = ddPair.first.notZero();
                Bdd<LibraryType> vectorDdNotZero = ddPair.second.notZero();

                std::vector<uint64_t> tmpRowIndications = matrixDdNotZero.template toAdd<uint_fast64_t>().sumAbstract(columnMetaVariables).toVector(rowOdd);
                for (uint64_t offset = 0; offset < tmpRowIndications.size(); ++offset) {
                    rowIndications[rowGroupIndices[offset]] += tmpRowIndications[offset];
                }

                ddPair.second.internalAdd.composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, explicitVector, std::plus<ValueType>());
                
                statesWithGroupEnabled[i] = (matrixDdNotZero.existsAbstract(columnMetaVariables) || vectorDdNotZero).template toAdd<uint_fast64_t>();
                stateToRowGroupCount += statesWithGroupEnabled[i];
                statesWithGroupEnabled[i].composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::plus<uint_fast64_t>());
            }
            
            // Since we modified the rowGroupIndices, we need to restore the correct values.
            stateToRowGroupCount.composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::minus<uint_fast64_t>());

            // Now that we computed the number of entries in each row, compute the corresponding offsets in the entry vector.
            tmp = 0;
            tmp2 = 0;
            for (uint_fast64_t i = 1; i < rowIndications.size(); ++i) {
                tmp2 = rowIndications[i];
                rowIndications[i] = rowIndications[i - 1] + tmp;
                std::swap(tmp, tmp2);
            }
            rowIndications[0] = 0;
            
            // Now actually fill the entry vector.
            for (uint_fast64_t i = 0; i < groups.size(); ++i) {
                auto const& dd = groups[i].first;
                
                dd.internalAdd.toMatrixComponents(rowGroupIndices, rowIndications, columnsAndValues, rowOdd, columnOdd, ddRowVariableIndices, ddColumnVariableIndices, true);
                statesWithGroupEnabled[i].composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::plus<uint_fast64_t>());
            }
            
            // Since we modified the rowGroupIndices, we need to restore the correct values.
            stateToRowGroupCount.composeWithExplicitVector(rowOdd, ddRowVariableIndices, rowGroupIndices, std::minus<uint_fast64_t>());
            
            // Since the last call to toMatrixRec modified the rowIndications, we need to restore the correct values.
            for (uint_fast64_t i = rowIndications.size() - 1; i > 0; --i) {
                rowIndications[i] = rowIndications[i - 1];
            }
            rowIndications[0] = 0;
                        
            return std::make_pair(storm::storage::SparseMatrix<ValueType>(columnOdd.getTotalOffset(), std::move(rowIndications), std::move(columnsAndValues), std::move(rowGroupIndices)), std::move(explicitVector));
        }

        template<DdType LibraryType, typename ValueType>
        void Add<LibraryType, ValueType>::exportToDot(std::string const& filename, bool showVariablesIfPossible) const {
            internalAdd.exportToDot(filename, this->getDdManager().getDdVariableNames(), showVariablesIfPossible);
        }
        
        template<DdType LibraryType, typename ValueType>
        AddIterator<LibraryType, ValueType> Add<LibraryType, ValueType>::begin(bool enumerateDontCareMetaVariables) const {
            uint_fast64_t numberOfDdVariables = 0;
            for (auto const& metaVariable : this->getContainedMetaVariables()) {
                auto const& ddMetaVariable = this->getDdManager().getMetaVariable(metaVariable);
                numberOfDdVariables += ddMetaVariable.getNumberOfDdVariables();
            }
            
            return internalAdd.begin(this->getDdManager(), Bdd<LibraryType>::getCube(this->getDdManager(), this->getContainedMetaVariables()).getInternalBdd(), numberOfDdVariables, this->getContainedMetaVariables(), enumerateDontCareMetaVariables);
        }
        
        template<DdType LibraryType, typename ValueType>
        AddIterator<LibraryType, ValueType> Add<LibraryType, ValueType>::end() const {
            return internalAdd.end(this->getDdManager());
        }
        
        template<DdType LibraryType, typename ValueType>
        std::ostream& operator<<(std::ostream& out, Add<LibraryType, ValueType> const& add) {
            out << "ADD with " << add.getNonZeroCount() << " nnz, " << add.getNodeCount() << " nodes, " << add.getLeafCount() << " leaves" << std::endl;
            std::vector<std::string> variableNames;
            for (auto const& variable : add.getContainedMetaVariables()) {
                variableNames.push_back(variable.getName());
            }
            out << "contained variables: " << boost::algorithm::join(variableNames, ", ") << std::endl;
            return out;
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType> Add<LibraryType, ValueType>::fromVector(DdManager<LibraryType> const& ddManager, std::vector<ValueType> const& values, Odd const& odd, std::set<storm::expressions::Variable> const& metaVariables) {
            return Add<LibraryType, ValueType>(ddManager, InternalAdd<LibraryType, ValueType>::fromVector(ddManager.getInternalDdManagerPointer(), values, odd, ddManager.getSortedVariableIndices(metaVariables)), metaVariables);
        }
        
        template<DdType LibraryType, typename ValueType>
        Bdd<LibraryType> Add<LibraryType, ValueType>::toBdd() const {
            return this->notZero();
        }
        
        template<DdType LibraryType, typename ValueType>
        Odd Add<LibraryType, ValueType>::createOdd() const {
            return internalAdd.createOdd(this->getSortedVariableIndices());
        }
        
        template<DdType LibraryType, typename ValueType>
        InternalAdd<LibraryType, ValueType> const& Add<LibraryType, ValueType>::getInternalAdd() const {
            return internalAdd;
        }
        
        template<DdType LibraryType, typename ValueType>
        InternalDdManager<LibraryType> const& Add<LibraryType, ValueType>::getInternalDdManager() const {
            return internalAdd.getInternalDdManager();
        }

        template<DdType LibraryType, typename ValueType>
        Add<LibraryType, ValueType>::operator InternalAdd<LibraryType, ValueType>() const {
            return internalAdd;
        }
        
		template<DdType LibraryType, typename ValueType>
        template<typename TargetValueType>
		Add<LibraryType, TargetValueType> Add<LibraryType, ValueType>::toValueType() const {
            if (std::is_same<TargetValueType, ValueType>::value) {
                return *this;
            }
			STORM_LOG_THROW(false, storm::exceptions::InvalidOperationException, "Cannot convert this ADD to the target type.");
		}

#ifdef STORM_HAVE_CARL
		template<>
        template<>
		Add<storm::dd::DdType::Sylvan, double> Add<storm::dd::DdType::Sylvan, storm::RationalFunction>::toValueType() const {
			return Add<storm::dd::DdType::Sylvan, double>(this->getDdManager(), internalAdd.toValueType<double>(), this->getContainedMetaVariables());
		}
#endif
		
        template class Add<storm::dd::DdType::CUDD, double>;
        template class Add<storm::dd::DdType::CUDD, uint_fast64_t>;

        template class Add<storm::dd::DdType::Sylvan, double>;
        template class Add<storm::dd::DdType::Sylvan, uint_fast64_t>;
        
#ifdef STORM_HAVE_CARL
        template class Add<storm::dd::DdType::Sylvan, storm::RationalNumber>;
		template class Add<storm::dd::DdType::Sylvan, storm::RationalFunction>;
#endif
    }
}
