#pragma once

/*

String Helpers

*/

//----

#include "ofMain.h"

#include <unordered_set>
#include <regex>

namespace ofxSurfingHelpers {

	//--------------------------------------------------------------
	inline void removeLeadingSpaces(std::vector<std::string>& vec) {
		for (auto& str : vec) {
			str.erase(0, str.find_first_not_of(' '));
		}
	};

	//--------------------------------------------------------------
	inline int countNewlines(std::string str) {
		int count = 0;
		for (char c : str) {
			if (c == '\n') {
				count++;
			}
		}
		return count;
	};

	//--------------------------------------------------------------
	inline string loadFileText(string path)
	{
		string p = ofToDataPath(path, true);

		string text = "";

		char* pathChars = (char*)(p.c_str());

		//--

		ofLogNotice("ofxSurfingTextSubtitle") << "load ifstream pathChars: " << ofToString(pathChars);

		std::ifstream t(pathChars);
		if (t.good())
		{
			string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			text = str;

			ofLogNotice("ofxSurfingTextSubtitle") << "loaded file: " << ofToString(pathChars);
		}
		else
		{
			ofLogNotice("ofxSurfingTextSubtitle") << "file not found! " << ofToString(pathChars);
		}

		return text;
	};

	//#include <unordered_set>//required
	// will split blocks checking new lines '\n'
	//--------------------------------------------------------------
	inline std::vector<std::string> splitTextBlocks(const std::string& s) {
		std::vector<std::string> blocks;
		std::string current_block;
		bool inside_block = false; // Keeps track of whether we're currently inside a block

		std::unordered_set<std::string> exceptions{ " s.", " d.", " C." }; // List of exceptions to ignore
		// exceptions will by pass the separation of text blocks.
		//TODO: must add more cases from English. these above exceptions are from Spanish language.

		for (char c : s) {
			if (c == '\n' || c == '\r') {
				continue; // Ignore newline characters
			}

			current_block += c;

			if (c == '.') {
				if (exceptions.count(current_block.substr(current_block.length() - 3)) > 0) { // Check if the current block ends with an exception
					continue; // Skip the block termination and keep reading
				}
				else {
					inside_block = false;
					blocks.push_back(current_block);
					current_block.clear();
				}
			}
			else {
				inside_block = true;
			}

			if (current_block.length() > 10000) {
				std::cerr << "Block size limit exceeded" << std::endl;
				break;
			}
		}

		if (!current_block.empty()) {
			blocks.push_back(current_block);
		}

		// remove starting spaces on each text blocks
		removeLeadingSpaces(blocks);

		return blocks;
	};

	// regex_replace
	// https://stackoverflow.com/questions/3654617/remove-number-then-a-space-from-the-start-of-a-string
	/*
			/ ^ \d + \s + /
			It looks for a number of any size in the beginning of a string^ \d +
			Then looks for a patch of whitespace after it \s +
			When you use a backslash before certain letters it represents something...
			\d represents a digit 0, 1, 2, 3, 4, 5, 6, 7, 8, 9.
			\s represents a space .
			Add a plus sign(+) to the end and you can have...
			\d + a series of digits(number)
			\s + multiple spaces(typos etc.)
	*/
	//--------------------------------------------------------------
	static void removeNumberPatternsFromBlocks(std::vector<std::string>& blocks) {
		const std::string pattern = "/^\d+\s+/";
		//const std::string pattern = "^\\d+\\s+";
		//const std::string pattern = "\\d+\\. ";
		//const std::string pattern = "^\d+";

		for (auto& block : blocks) {
			std::regex_replace(block, std::regex(pattern), "");

			////TODO:
			////remove '.'. must be at end only!
			////fix errors
			//ofStringReplace(block, ".", "");
			//ofStringReplace(block, "\"", "");
		}
	};

	//--------------------------------------------------------------
	static std::vector<std::string> splitTextBlocksNumbered(std::string paragraph) {
		std::vector<std::string> blocks;
		std::string currentBlock = "";
		int currentNumber = 1;
		bool hasValidBlock = false;

		for (std::size_t i = 0; i < paragraph.size(); i++) {
			char c = paragraph[i];

			if (c == ' ' && currentBlock == "") {
				// Ignore leading spaces
				continue;
			}

			if (isdigit(c) && (i == 0 || paragraph[i - 1] == ' ')) {
				// Start of a new block
				currentNumber++;
				//// Skip block number
				//i += 2;
			}

			//if (currentNumber > 50) {
			//	// We only care about blocks up to 50
			//	break;
			//}

			if (c == '\n') {
				// Add block to vector
				if (hasValidBlock) {
					blocks.push_back(currentBlock);
				}
				// Reset block string
				currentBlock = "";
				hasValidBlock = false;
			}
			else {
				// Add character to current block
				currentBlock += c;
				hasValidBlock = true;
			}
		}

		// Store final block
		if (hasValidBlock) {
			blocks.push_back(currentBlock);
		}

		//TODO:
		// Extra step. Remove numbers pattern
		//removeNumberPatternsFromBlocks(blocks);

		return blocks;
	};

	//----

	/*
	//TODO:
	//text file must be correctly encoded!
	//if not we need to fix utf8 chars..
	std::string remove_invalid_code_points(const std::string& s) {
		std::string result;

		// Loop through the string and append only valid code points to the result string
		for (auto it = s.begin(); it != s.end(); ) {
			try {
				uint32_t code_point = utf8::next(it, s.end());//!

				//result += utf8::encode(code_point);?
				//result += (code_point);
				result += ofUTF8ToString(code_point);
			}
			catch (utf8::invalid_code_point) {
				// Ignore invalid code points
			}
		}

		return result;
	};
	*/



//--

//TODO: break lines for text formatting by setting a with of amount of of chars.
//https://stackoverflow.com/questions/6891652/formatting-a-string-into-multiple-lines-of-a-specific-length-in-c-c

	inline string splitInLines(string source, std::size_t width, string whitespace = " \t\r")
	{
		std::size_t  currIndex = width - 1;
		std::size_t  sizeToElim;
		while (currIndex < source.length())
		{
			currIndex = source.find_last_of(whitespace, currIndex + 1);
			if (currIndex == string::npos)
				break;
			currIndex = source.find_last_not_of(whitespace, currIndex);
			if (currIndex == string::npos)
				break;
			sizeToElim = source.find_first_not_of(whitespace, currIndex + 1) - currIndex - 1;
			source.replace(currIndex + 1, sizeToElim, "\n");
			currIndex += (width + 1); //due to the recently inserted "\n"
		}
		return source;
	}

	/*
	int main() {
		string source = "Shankle drumstick corned beef, chuck turkey chicken pork chop venison beef strip steak cow sausage. Tail short loin shoulder ball tip, jowl drumstick rump. Tail tongue ball tip meatloaf, bresaola short loin tri-tip fatback pork loin sirloin shank flank biltong. Venison short loin andouille.";
		string result = splitInLines(source, 60);
		std::cout << result;
		return 0;
	}
	*/

}; // ofxSurfingHelpers