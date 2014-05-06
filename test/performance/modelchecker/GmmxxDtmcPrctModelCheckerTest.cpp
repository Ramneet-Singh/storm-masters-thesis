#include "gtest/gtest.h"
#include "storm-config.h"
#include "src/settings/Settings.h"
#include "src/settings/InternalOptionMemento.h"
#include "src/modelchecker/prctl/SparseDtmcPrctlModelChecker.h"
#include "src/solver/GmmxxLinearEquationSolver.h"
#include "src/parser/AutoParser.h"

TEST(GmmxxDtmcPrctlModelCheckerTest, Crowds) {
	storm::settings::Settings* s = storm::settings::Settings::getInstance();
	storm::settings::InternalOptionMemento deadlockOption("fixDeadlocks", true);
	ASSERT_TRUE(s->isSet("fixDeadlocks"));
	std::shared_ptr<storm::models::AbstractModel<double>> abstractModel = storm::parser::AutoParser::parseModel(STORM_CPP_BASE_PATH "/examples/dtmc/crowds/crowds20_5.tra", STORM_CPP_BASE_PATH "/examples/dtmc/crowds/crowds20_5.lab", "", "");

	ASSERT_EQ(abstractModel->getType(), storm::models::DTMC);

	std::shared_ptr<storm::models::Dtmc<double>> dtmc = abstractModel->as<storm::models::Dtmc<double>>();

	ASSERT_EQ(dtmc->getNumberOfStates(), 2036647ull);
	ASSERT_EQ(dtmc->getNumberOfTransitions(), 7362293ull);

	storm::modelchecker::prctl::SparseDtmcPrctlModelChecker<double> mc(*dtmc, new storm::solver::GmmxxLinearEquationSolver<double>());

	storm::property::prctl::Ap<double>* apFormula = new storm::property::prctl::Ap<double>("observe0Greater1");
	storm::property::prctl::Eventually<double>* eventuallyFormula = new storm::property::prctl::Eventually<double>(apFormula);
	storm::property::prctl::ProbabilisticNoBoundOperator<double>* probFormula = new storm::property::prctl::ProbabilisticNoBoundOperator<double>(eventuallyFormula);

    LOG4CPLUS_WARN(logger, "Model Checking P=? [F observe0Greater1] on crowds/crowds20_5...");
	std::vector<double> result = probFormula->check(mc);
    LOG4CPLUS_WARN(logger, "Done.");

	ASSERT_LT(std::abs(result[0] - 0.2296800237), s->getOptionByLongName("precision").getArgument(0).getValueAsDouble());

	delete probFormula;

	apFormula = new storm::property::prctl::Ap<double>("observeIGreater1");
	eventuallyFormula = new storm::property::prctl::Eventually<double>(apFormula);
	probFormula = new storm::property::prctl::ProbabilisticNoBoundOperator<double>(eventuallyFormula);

    LOG4CPLUS_WARN(logger, "Model Checking P=? [F observeIGreater1] on crowds/crowds20_5...");
	result = probFormula->check(mc);
    LOG4CPLUS_WARN(logger, "Done.");
    
	ASSERT_LT(std::abs(result[0] - 0.05073232193), s->getOptionByLongName("precision").getArgument(0).getValueAsDouble());

	delete probFormula;

	apFormula = new storm::property::prctl::Ap<double>("observeOnlyTrueSender");
	eventuallyFormula = new storm::property::prctl::Eventually<double>(apFormula);
	probFormula = new storm::property::prctl::ProbabilisticNoBoundOperator<double>(eventuallyFormula);

    LOG4CPLUS_WARN(logger, "Model Checking P=? [F observeOnlyTrueSender] on crowds/crowds20_5...");
	result = probFormula->check(mc);
    LOG4CPLUS_WARN(logger, "Done.");

	ASSERT_LT(std::abs(result[0] - 0.22742171078), s->getOptionByLongName("precision").getArgument(0).getValueAsDouble());

	delete probFormula;
}


TEST(GmmxxDtmcPrctlModelCheckerTest, SynchronousLeader) {
	storm::settings::Settings* s = storm::settings::Settings::getInstance();
	storm::settings::InternalOptionMemento deadlockOption("fixDeadlocks", true);
	ASSERT_TRUE(s->isSet("fixDeadlocks"));
	std::shared_ptr<storm::models::AbstractModel<double>> abstractModel = storm::parser::AutoParser::parseModel(STORM_CPP_BASE_PATH "/examples/dtmc/synchronous_leader/leader6_8.tra", STORM_CPP_BASE_PATH "/examples/dtmc/synchronous_leader/leader6_8.lab", "", STORM_CPP_BASE_PATH "/examples/dtmc/synchronous_leader/leader6_8.pick.trans.rew");

	ASSERT_EQ(abstractModel->getType(), storm::models::DTMC);


	std::shared_ptr<storm::models::Dtmc<double>> dtmc = abstractModel->as<storm::models::Dtmc<double>>();

	ASSERT_EQ(dtmc->getNumberOfStates(), 1312334ull);
	ASSERT_EQ(dtmc->getNumberOfTransitions(), 1574477ull);

	storm::modelchecker::prctl::SparseDtmcPrctlModelChecker<double> mc(*dtmc, new storm::solver::GmmxxLinearEquationSolver<double>());

	storm::property::prctl::Ap<double>* apFormula = new storm::property::prctl::Ap<double>("elected");
	storm::property::prctl::Eventually<double>* eventuallyFormula = new storm::property::prctl::Eventually<double>(apFormula);
	storm::property::prctl::ProbabilisticNoBoundOperator<double>* probFormula = new storm::property::prctl::ProbabilisticNoBoundOperator<double>(eventuallyFormula);

    LOG4CPLUS_WARN(logger, "Model Checking P=? [F elected] on synchronous_leader/leader6_8...");
	std::vector<double> result = probFormula->check(mc);
    LOG4CPLUS_WARN(logger, "Done.");

	ASSERT_LT(std::abs(result[0] - 1.0), s->getOptionByLongName("precision").getArgument(0).getValueAsDouble());

	delete probFormula;

	apFormula = new storm::property::prctl::Ap<double>("elected");
	storm::property::prctl::BoundedUntil<double>* boundedUntilFormula = new storm::property::prctl::BoundedUntil<double>(new storm::property::prctl::Ap<double>("true"), apFormula, 20);
	probFormula = new storm::property::prctl::ProbabilisticNoBoundOperator<double>(boundedUntilFormula);

    LOG4CPLUS_WARN(logger, "Model Checking P=? [F<=20 elected] on synchronous_leader/leader6_8...");
	result = probFormula->check(mc);
    LOG4CPLUS_WARN(logger, "Done.");

	ASSERT_LT(std::abs(result[0] - 0.9993949793), s->getOptionByLongName("precision").getArgument(0).getValueAsDouble());

	delete probFormula;

	apFormula = new storm::property::prctl::Ap<double>("elected");
	storm::property::prctl::ReachabilityReward<double>* reachabilityRewardFormula = new storm::property::prctl::ReachabilityReward<double>(apFormula);
	storm::property::prctl::RewardNoBoundOperator<double>* rewardFormula = new storm::property::prctl::RewardNoBoundOperator<double>(reachabilityRewardFormula);

    LOG4CPLUS_WARN(logger, "Model Checking R=? [F elected] on synchronous_leader/leader6_8...");
	result = rewardFormula->check(mc);
    LOG4CPLUS_WARN(logger, "Done.");

	ASSERT_LT(std::abs(result[0] - 1.025106273), s->getOptionByLongName("precision").getArgument(0).getValueAsDouble());

	delete rewardFormula;
}
