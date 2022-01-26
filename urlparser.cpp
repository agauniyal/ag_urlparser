#include "urlparser.hpp"
#include "urlparser_schemes.hpp"
#include "urlparser_tlds.hpp"

#include <stdexcept>
#include <algorithm>

UrlParser::UrlParser(std::string inputURL) : _url(std::move(inputURL)) {
    // for https://user@pass:v1.abc.domain.co.in:8080/hello/world?abc=123&what=not#done

    auto modURL = extractScheme(_url);  // => user@pass:v1.abc.domain.co.in:8080/hello/world?abc=123&what=not#done
    modURL = extractFullPath(modURL);  // => user@pass:v1.abc.domain.co.in:8080
    modURL = extractPort(modURL);  // => user@pass:v1.abc.domain.co.in
    _host = extractAuth(modURL);  // => v1.abc.domain.co.in
    modURL = extractTLD(_host);  // => v1.abc.domain

    const bool isValidHost = !_tld.empty() && _host.substr(_tld.size() + 1).size() != 0;
    const bool isValidPort = _port >= -1 && _port <= 65535;
    if (!isValidHost || !isValidPort) {
        throw std::runtime_error("Cannot Parse Url: " + _url);
    }
}


constexpr std::string_view UrlParser::extractFullPath(const std::string_view& modURL) {
    const auto fullPathPos = modURL.find_first_of("/?");
    if (fullPathPos != std::string_view::npos) {
        _fullPath = modURL.substr(fullPathPos);
        return modURL.substr(0, fullPathPos);
    }
    return modURL;
}

constexpr std::string_view UrlParser::extractScheme(const std::string_view& modURL) {
    for (const std::string_view scheme : SchemeList) {
        if (modURL.starts_with(scheme)) {
            _scheme = modURL.substr(0, scheme.size());
            return modURL.substr(_scheme.size());
        }
    }
    return modURL;
}

constexpr std::string_view UrlParser::extractAuth(const std::string_view& modURL) {
    const auto authPos = modURL.find_first_of('@');
    if (authPos != std::string_view::npos) {
        _auth = modURL.substr(0, authPos);
        return modURL.substr(authPos + 1);
    }
    return modURL;
}

constexpr std::string_view UrlParser::extractPort(const std::string_view& modURL) {
    const auto portPos = modURL.rfind(':');
    bool isValidPortPos = portPos != std::string_view::npos;
    isValidPortPos = isValidPortPos && std::all_of(modURL.cbegin() + portPos + 1, modURL.cend(), isdigit);
    if (isValidPortPos) {
        _port = std::stoi(modURL.substr(portPos + 1).data());
        return modURL.substr(0, portPos);
    }
    return modURL;
}

constexpr std::string_view UrlParser::extractTLD(const std::string_view& modURL) {
    for (const std::string_view tld : URL_TLDs) {
        if (modURL.ends_with(tld)) {
            _tld = modURL.substr(modURL.size() - tld.size());
            return modURL.substr(0, modURL.size() - tld.size() - 1);
        }
    }
    return modURL;
}
