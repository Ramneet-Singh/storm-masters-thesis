/*
 * PrctlFileParser.cpp
 *
 *  Created on: 06.02.2013
 *      Author: Thomas Heinemann
 */

#include <sstream>

#include "PrctlFileParser.h"
#include "PrctlParser.h"

namespace storm {
namespace parser {

PrctlFileParser::PrctlFileParser() {
	//Intentionally left empty
}

PrctlFileParser::~PrctlFileParser() {
	//intentionally left empty
}

std::list<storm::property::prctl::AbstractPrctlFormula<double>*> PrctlFileParser::parseFormulas(std::string filename) {
	// Open file
	std::ifstream inputFileStream(filename, std::ios::in);

	if (!inputFileStream.is_open()) {
		std::string message = "Error while opening file ";
		throw storm::exceptions::FileIoException() << message << filename;
	}

	std::list<storm::property::prctl::AbstractPrctlFormula<double>*> result;

	while(!inputFileStream.eof()) {
		std::string line;
		//The while loop reads the input file line by line
		while (std::getline(inputFileStream, line)) {
			PrctlParser parser(line);
			result.push_back(parser.getFormula());
		}
	}

	return result;
}

} /* namespace parser */
} /* namespace storm */
