#include "urlparser.hpp"
#include "test_generator.hpp"

#include <iostream>
#include <vector>
#include <string>

// Make reverse trie to find valid tlds faster

bool compareUrlParser(const UrlParser& p, const UrlComponents& c) {
    auto userPass = p.auth().first.empty() ? "" : std::string(p.auth().first) + ":" + std::string(p.auth().second);
    const bool isSchemeSame = p.scheme() == c.scheme;
    const bool isAuthSame = userPass == c.auth;
    const bool isSubSame = p.subdomain() == c.subdomain;
    const bool isNameSame = p.name() == c.name;
    const bool isTldSame = p.tld() == c.tld;
    const bool isPathSame = p.path() == c.path;
    const bool isQuerySame = p.query() == c.query;
    const bool isFragSame = p.fragment() == c.fragment;
    const bool isDomainSame = p.domain() == c.domain;

    return isAuthSame && isFragSame && isSchemeSame && isTldSame && isNameSame && isPathSame && isQuerySame
      && isSubSame;
}


int main() {
    using std::cout, std::endl, std::boolalpha;
    for (const auto& comp : generateValidTestCases()) {
        UrlParser p(comp.url);
        if (!compareUrlParser(p, comp)) {
            cout << comp.url << endl;
        }
    }
}
