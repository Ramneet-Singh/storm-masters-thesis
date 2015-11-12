#ifndef STORM_STORAGE_DD_BDD_H_
#define STORM_STORAGE_DD_BDD_H_

#include "src/storage/dd/Dd.h"
#include "src/storage/dd/DdType.h"

#include "src/storage/dd/cudd/InternalCuddBdd.h"

namespace storm {
    namespace dd {
        template<DdType LibraryType, typename ValueType>
        class Add;
        
        template<DdType LibraryType>
        class Bdd : public Dd<LibraryType> {
        public:
            friend class DdManager<LibraryType>;
            
            // Instantiate all copy/move constructors/assignments with the default implementation.
            Bdd() = default;
            Bdd(Bdd<LibraryType> const& other) = default;
            Bdd& operator=(Bdd<LibraryType> const& other) = default;
            Bdd(Bdd<LibraryType>&& other) = default;
            Bdd& operator=(Bdd<LibraryType>&& other) = default;
            
            /*!
             * Retrieves whether the two BDDs represent the same function.
             *
             * @param other The BDD that is to be compared with the current one.
             * @return True if the BDDs represent the same function.
             */
            bool operator==(Bdd<LibraryType> const& other) const;
            
            /*!
             * Retrieves whether the two BDDs represent different functions.
             *
             * @param other The BDD that is to be compared with the current one.
             * @return True if the BDDs represent the different functions.
             */
            bool operator!=(Bdd<LibraryType> const& other) const;
            
            /*!
             * Performs an if-then-else with the given operands, i.e. maps all valuations that are mapped to a non-zero
             * function value to the function values specified by the first DD and all others to the function values
             * specified by the second DD.
             *
             * @param thenBdd The BDD defining the 'then' part.
             * @param elseBdd The BDD defining the 'else' part.
             * @return The resulting BDD.
             */
            Bdd<LibraryType> ite(Bdd<LibraryType> const& thenBdd, Bdd<LibraryType> const& elseBdd) const;
            
            /*!
             * Performs a logical or of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical or of the operands.
             */
            Bdd<LibraryType> operator||(Bdd<LibraryType> const& other) const;
            
            /*!
             * Performs a logical or of the current and the given BDD and assigns it to the current BDD.
             *
             * @param other The second BDD used for the operation.
             * @return A reference to the current BDD after the operation
             */
            Bdd<LibraryType>& operator|=(Bdd<LibraryType> const& other);
            
            /*!
             * Performs a logical and of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical and of the operands.
             */
            Bdd<LibraryType> operator&&(Bdd<LibraryType> const& other) const;
            
            /*!
             * Performs a logical and of the current and the given BDD and assigns it to the current BDD.
             *
             * @param other The second BDD used for the operation.
             * @return A reference to the current BDD after the operation
             */
            Bdd<LibraryType>& operator&=(Bdd<LibraryType> const& other);
            
            /*!
             * Performs a logical iff of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical iff of the operands.
             */
            Bdd<LibraryType> iff(Bdd<LibraryType> const& other) const;
            
            /*!
             * Performs a logical exclusive-or of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical exclusive-or of the operands.
             */
            Bdd<LibraryType> exclusiveOr(Bdd<LibraryType> const& other) const;
            
            /*!
             * Performs a logical implication of the current and the given BDD.
             *
             * @param other The second BDD used for the operation.
             * @return The logical implication of the operands.
             */
            Bdd<LibraryType> implies(Bdd<LibraryType> const& other) const;
            
            /*!
             * Logically inverts the current BDD.
             *
             * @return The resulting BDD.
             */
            Bdd<LibraryType> operator!() const;
            
            /*!
             * Logically complements the current BDD.
             *
             * @return A reference to the current BDD after the operation.
             */
            Bdd<LibraryType>& complement();
            
            /*!
             * Existentially abstracts from the given meta variables.
             *
             * @param metaVariables The meta variables from which to abstract.
             */
            Bdd<LibraryType> existsAbstract(std::set<storm::expressions::Variable> const& metaVariables) const;
            
            /*!
             * Universally abstracts from the given meta variables.
             *
             * @param metaVariables The meta variables from which to abstract.
             */
            Bdd<LibraryType> universalAbstract(std::set<storm::expressions::Variable> const& metaVariables) const;
            
            /*!
             * Computes the logical and of the current and the given BDD and existentially abstracts from the given set
             * of variables.
             *
             * @param other The second BDD for the logical and.
             * @param existentialVariables The variables from which to existentially abstract.
             * @return A BDD representing the result.
             */
            Bdd<LibraryType> andExists(Bdd<LibraryType> const& other, std::set<storm::expressions::Variable> const& existentialVariables) const;
            
            /*!
             * Computes the constraint of the current BDD with the given constraint. That is, the function value of the
             * resulting BDD will be the same as the current ones for all assignments mapping to one in the constraint
             * and may be different otherwise.
             *
             * @param constraint The constraint to use for the operation.
             * @return The resulting BDD.
             */
            Bdd<LibraryType> constrain(Bdd<LibraryType> const& constraint) const;
            
            /*!
             * Computes the restriction of the current BDD with the given constraint. That is, the function value of the
             * resulting DD will be the same as the current ones for all assignments mapping to one in the constraint
             * and may be different otherwise.
             *
             * @param constraint The constraint to use for the operation.
             * @return The resulting BDD.
             */
            Bdd<LibraryType> restrict(Bdd<LibraryType> const& constraint) const;
            
            /*!
             * Swaps the given pairs of meta variables in the BDD. The pairs of meta variables must be guaranteed to have
             * the same number of underlying BDD variables.
             *
             * @param metaVariablePairs A vector of meta variable pairs that are to be swapped for one another.
             * @return The resulting BDD.
             */
            Bdd<LibraryType> swapVariables(std::vector<std::pair<storm::expressions::Variable, storm::expressions::Variable>> const& metaVariablePairs) const;
            
            /*!
             * Retrieves whether this DD represents the constant one function.
             *
             * @return True if this DD represents the constant one function.
             */
            bool isOne() const;
            
            /*!
             * Retrieves whether this DD represents the constant zero function.
             *
             * @return True if this DD represents the constant zero function.
             */
            bool isZero() const;
            
            /*!
             * Converts a BDD to an equivalent ADD.
             *
             * @return The corresponding ADD.
             */
            template<typename ValueType>
            Add<LibraryType, ValueType> toAdd() const;
            
            /*!
             * Converts the BDD to a bit vector. The given offset-labeled DD is used to determine the correct row of
             * each entry.
             *
             * @param rowOdd The ODD used for determining the correct row.
             * @return The bit vector that is represented by this BDD.
             */
            storm::storage::BitVector toVector(storm::dd::Odd<LibraryType> const& rowOdd) const;
            
            virtual Bdd<LibraryType> getSupport() const override;
            
            virtual uint_fast64_t getNonZeroCount() const override;
            
            virtual uint_fast64_t getLeafCount() const override;
            
            virtual uint_fast64_t getNodeCount() const override;
            
            virtual uint_fast64_t getIndex() const override;
            
            virtual void exportToDot(std::string const& filename) const override;
            
            /*!
             * Retrieves the cube of all given meta variables.
             *
             * @param metaVariables The variables for which to create the cube.
             * @return The resulting cube.
             */
            Bdd<LibraryType> getCube(std::set<storm::expressions::Variable> const& metaVariables) const;
            
        private:
            /*!
             * We provide a conversion operator from the BDD to its internal type to ease calling the internal functions.
             */
            operator InternalBdd<LibraryType>();
            operator InternalBdd<LibraryType> const() const;
            
            /*!
             * Creates a DD that encapsulates the given CUDD ADD.
             *
             * @param ddManager The manager responsible for this DD.
             * @param cuddBdd The CUDD BDD to store.
             * @param containedMetaVariables The meta variables that appear in the DD.
             */
            Bdd(std::shared_ptr<DdManager<DdType::CUDD> const> ddManager, InternalBdd<LibraryType> const& internalBdd, std::set<storm::expressions::Variable> const& containedMetaVariables = std::set<storm::expressions::Variable>());
            
            /*!
             * Constructs a BDD representation of all encodings that are in the requested relation with the given value.
             *
             * @param ddManager The DD manager responsible for the resulting BDD.
             * @param explicitValues The explicit values to compare to the given value.
             * @param odd The ODD used for the translation from the explicit representation to a symbolic one.
             * @param metaVariables The meta variables to use for the symbolic encoding.
             * @param comparisonType The relation that needs to hold for the values (wrt. to the given value).
             * @param value The value to compare with.
             */
            static Bdd<LibraryType> fromVector(std::shared_ptr<DdManager<LibraryType> const> ddManager, std::vector<double> const& explicitValues, storm::dd::Odd<LibraryType> const& odd, std::set<storm::expressions::Variable> const& metaVariables, storm::logic::ComparisonType comparisonType, double value);

            /*!
             * Builds a BDD representing the values that make the given filter function evaluate to true.
             *
             * @param ddManager The manager responsible for the BDD.
             * @param values The values that are to be checked against the filter function.
             * @param odd The ODD used for the translation.
             * @param metaVariables The meta variables used for the translation.
             * @param filter The filter that evaluates whether an encoding is to be mapped to 0 or 1.
             * @return The resulting (CUDD) BDD.
             */
            template<typename ValueType>
            static Bdd<LibraryType> fromVector(std::shared_ptr<DdManager<DdType::CUDD> const> ddManager, std::vector<ValueType> const& values, Odd<DdType::CUDD> const& odd, std::set<storm::expressions::Variable> const& metaVariables, std::function<bool (ValueType const&)> const& filter);
            
            // The internal BDD that depends on the chosen library.
            InternalBdd<LibraryType> internalBdd;
        };
    }
}

#endif /* STORM_STORAGE_DD_BDD_H_ */