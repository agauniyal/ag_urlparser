#pragma once

#include <string>
#include <vector>

using StringList = std::vector<std::string>;

namespace Valid {
StringList listOfSchemes = { "", "https://", "http://" };
StringList listOfAuth = { "", "username:password@" };
StringList listOfSubDomains = { "", "hello.", "网络." };
StringList listOfNames = { "google", "网络" };
StringList listOfTlds = { ".商城", ".ca" };
StringList listOfPorts = { "", ":1", ":65535", ":80" };
StringList listOfPath = { "", "//", "/abc", "/abc/", "/abcd/bcde", "/abcd/bcde//" };
StringList listOfQuery = { "", "?abcd=bcde", "?abcd=bcde&cd=1" };
StringList listOfFragment = { "", "#abc" };
}  // namespace Valid


struct UrlComponents {
    std::string scheme;
    std::string auth;
    std::string subdomain;
    std::string name;
    std::string tld;
    std::string domain;
    std::string port;
    std::string path;
    std::string query;
    std::string fragment;
    std::string url;
};


std::vector<UrlComponents> generateValidTestCases() {
    std::vector<UrlComponents> testCases;
    for (const auto& scheme : Valid::listOfSchemes) {
        UrlComponents c;
        c.scheme = scheme;
        c.url = scheme;
        testCases.push_back(c);
    }

    std::vector<UrlComponents> temp;
    for (const auto& auth : Valid::listOfAuth) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            c.auth = auth.substr(0, auth.size()-1);
            c.url.append(auth);
            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    for (const auto& subdomain : Valid::listOfSubDomains) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            c.subdomain = subdomain.empty() ? subdomain : subdomain.substr(0, subdomain.size() - 1);
            c.url.append(subdomain);
            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    for (const auto& name : Valid::listOfNames) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            c.name = name;
            c.domain.append(name);
            c.url.append(name);
            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    for (const auto& tld : Valid::listOfTlds) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            c.tld = tld.substr(1);
            c.domain.append(tld);
            c.url.append(tld);
            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    for (const auto& port : Valid::listOfPorts) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            c.port = port.empty() ? port: port.substr(1);
            c.url.append(port);
            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    for (const auto& path : Valid::listOfPath) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            c.path = path;
            c.url.append(path);
            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    for (const auto& query : Valid::listOfQuery) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            c.query = query;
            c.url.append(query);
            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    for (const auto& fragment : Valid::listOfFragment) {
        for (const auto& testCase : testCases) {
            UrlComponents c = testCase;
            if ((!c.path.ends_with('/') || c.path.empty()) && c.query.empty() && !fragment.empty()) {
                c.path.append(1, '/');
                c.url.append(1, '/');
            }
            c.fragment = fragment;
            c.url.append(fragment);

            temp.push_back(c);
        }
    }
    std::swap(testCases, temp);
    temp.clear();

    return testCases;
}
