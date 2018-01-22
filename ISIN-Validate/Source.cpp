
#include <string>
#include <regex>
#include <algorithm>
#include <numeric>
#include <sstream>

bool CheckFormat(const std::string& isin)
{
	std::regex isinRegEpx(R"([A-Z]{2}[A-Z0-9]{9}[0-9])");
	std::smatch match;
	return std::regex_match(isin, match, isinRegEpx);
}

std::string CodeISIN(const std::string& isin)
{
	std::string coded;
	int offset = 'A' - 10;
	for (auto ch : isin)
	{
		if (ch >= 'A' && ch <= 'Z')
		{
			std::stringstream ss;
			ss << static_cast<int>(ch) - offset;
			coded += ss.str();
		}
		else
		{
			coded.push_back(ch);
		}
	}

	return std::move(coded);
}

// from http://rosettacode.org/wiki/Luhn_test_of_credit_card_numbers#C.2B.2B11
bool luhn(const std::string& id)
{
	static const int m[10] = { 0,2,4,6,8,1,3,5,7,9 }; // mapping for rule 3
	bool is_odd_dgt = false;
	auto lambda = [&](int a, char c) {return a + ((is_odd_dgt = !is_odd_dgt) ? c - '0' : m[c - '0']);};
	int s = std::accumulate(id.rbegin(), id.rend(), 0, lambda);
	return 0 == s % 10;
}

bool CkeckISIN(const std::string& isin)
{
	if (!CheckFormat(isin))
		return false;

	std::string coded = CodeISIN(isin);
	return luhn(coded);
}


#include <iomanip>
#include <iostream>

int main()
{
	std::string isins[] = { "US0378331005", "US0373831005", "U50378331005",
							"US03378331005", "AU0000XVGZA3", "AU0000VXGZA3",
							"FR0000988040" };
	for (const auto& isin : isins)
	{
		std::cout << isin << std::boolalpha << " - " << CkeckISIN(isin) <<std::endl;
	}
	return 0;
}