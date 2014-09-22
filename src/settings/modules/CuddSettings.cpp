#include "src/settings/modules/CuddSettings.h"

#include "src/settings/SettingsManager.h"

namespace storm {
    namespace settings {
        namespace modules {
            
            const std::string moduleName = "cudd";
            const std::string precisionOptionName = "precision";
            const std::string maximalMemoryOptionName = "maxmem";
            const std::string reorderOptionName = "reorder";
            
            CuddSettings::CuddSettings(storm::settings::SettingsManager& settingsManager) : ModuleSettings(settingsManager, moduleName) {
                this->addOption(storm::settings::OptionBuilder(moduleName, precisionOptionName, true, "Sets the precision used by Cudd.").addArgument(storm::settings::ArgumentBuilder::createDoubleArgument("value", "The precision up to which to constants are considered to be different.").setDefaultValueDouble(1e-15).addValidationFunctionDouble(storm::settings::ArgumentValidators::doubleRangeValidatorExcluding(0.0, 1.0)).build()).build());
                
                this->addOption(storm::settings::OptionBuilder(moduleName, maximalMemoryOptionName, true, "Sets the upper bound of memory available to Cudd in MB.").addArgument(storm::settings::ArgumentBuilder::createUnsignedIntegerArgument("value", "The memory available to Cudd (0 means unlimited).").setDefaultValueUnsignedInteger(2048).build()).build());
                
                std::vector<std::string> reorderingTechniques;
                reorderingTechniques.push_back("none");
                reorderingTechniques.push_back("random");
                reorderingTechniques.push_back("randompivot");
                reorderingTechniques.push_back("sift");
                reorderingTechniques.push_back("siftconv");
                reorderingTechniques.push_back("ssift");
                reorderingTechniques.push_back("ssiftconv");
                reorderingTechniques.push_back("gsift");
                reorderingTechniques.push_back("gsiftconv");
                reorderingTechniques.push_back("win2");
                reorderingTechniques.push_back("win2conv");
                reorderingTechniques.push_back("win3");
                reorderingTechniques.push_back("win3conv");
                reorderingTechniques.push_back("win4");
                reorderingTechniques.push_back("win4conv");
                reorderingTechniques.push_back("annealing");
                reorderingTechniques.push_back("genetic");
                reorderingTechniques.push_back("exact");
                this->addOption(storm::settings::OptionBuilder(moduleName, reorderOptionName, true, "Sets the reordering technique used by Cudd.").addArgument(storm::settings::ArgumentBuilder::createStringArgument("method", "Sets which technique is used by Cudd's reordering routines. Must be in {none, random, randompivot, sift, siftconv, ssift, ssiftconv, gsift, gsiftconv, win2, win2conv, win3, win3conv, win4, win4conv, annealing, genetic, exact}.").setDefaultValueString("gsift").addValidationFunctionString(storm::settings::ArgumentValidators::stringInListValidator(reorderingTechniques)).build()).build());
            }
            
            double CuddSettings::getConstantPrecision() const {
                return this->getOption(precisionOptionName).getArgumentByName("value").getValueAsDouble();
            }
            
            uint_fast64_t CuddSettings::getMaximalMemory() const {
                return this->getOption(maximalMemoryOptionName).getArgumentByName("value").getValueAsUnsignedInteger();
            }
            
            CuddSettings::ReorderingTechnique CuddSettings::getReorderingTechnique() const {
                std::string reorderingTechniqueAsString = this->getOption(reorderOptionName).getArgumentByName("method").getValueAsString();
                if (reorderingTechniqueAsString == "none") {
                    return CuddSettings::ReorderingTechnique::None;
                } else if (reorderingTechniqueAsString == "random") {
                    return CuddSettings::ReorderingTechnique::Random;
                } else if (reorderingTechniqueAsString == "randompivot") {
                    return CuddSettings::ReorderingTechnique::RandomPivot;
                } else if (reorderingTechniqueAsString == "sift") {
                    return CuddSettings::ReorderingTechnique::Sift;
                } else if (reorderingTechniqueAsString == "siftconv") {
                    return CuddSettings::ReorderingTechnique::SiftConv;
                } else if (reorderingTechniqueAsString == "ssift") {
                    return CuddSettings::ReorderingTechnique::SymmetricSift;
                } else if (reorderingTechniqueAsString == "ssiftconv") {
                    return CuddSettings::ReorderingTechnique::SymmetricSiftConv;
                } else if (reorderingTechniqueAsString == "gsift") {
                    return CuddSettings::ReorderingTechnique::GroupSift;
                } else if (reorderingTechniqueAsString == "gsiftconv") {
                    return CuddSettings::ReorderingTechnique::GroupSiftConv;
                } else if (reorderingTechniqueAsString == "win2") {
                    return CuddSettings::ReorderingTechnique::Win2;
                } else if (reorderingTechniqueAsString == "win2conv") {
                    return CuddSettings::ReorderingTechnique::Win2Conv;
                } else if (reorderingTechniqueAsString == "win3") {
                    return CuddSettings::ReorderingTechnique::Win3;
                } else if (reorderingTechniqueAsString == "win3conv") {
                    return CuddSettings::ReorderingTechnique::Win3Conv;
                } else if (reorderingTechniqueAsString == "win4") {
                    return CuddSettings::ReorderingTechnique::Win4;
                } else if (reorderingTechniqueAsString == "win4conv") {
                    return CuddSettings::ReorderingTechnique::Win4Conv;
                } else if (reorderingTechniqueAsString == "annealing") {
                    return CuddSettings::ReorderingTechnique::Annealing;
                } else if (reorderingTechniqueAsString == "genetic") {
                    return CuddSettings::ReorderingTechnique::Genetic;
                } else if (reorderingTechniqueAsString == "exact") {
                    return CuddSettings::ReorderingTechnique::Exact;
                }
                LOG_THROW(false, storm::exceptions::IllegalArgumentValueException, "Illegal value '" << reorderingTechniqueAsString << "' set as reordering technique of Cudd.");
            }
            
        } // namespace modules
    } // namespace settings
} // namespace storm