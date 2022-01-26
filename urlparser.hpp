#pragma once

#include <string>
#include <string_view>
#include <optional>

class UrlParser final {
public:
    explicit UrlParser(std::string inputURL);

    constexpr std::string_view scheme() const noexcept {
        return _scheme;
    }
    constexpr std::string_view host() const noexcept {
        return _host;
    }
    constexpr std::string_view tld() const noexcept {
        return _tld;
    }
    constexpr std::string_view fullPath() const noexcept {
        return _fullPath;
    }
    constexpr const std::string& url() const noexcept {
        return _url;
    };
    constexpr int port() const noexcept {
        return _port;
    }

    constexpr std::string_view path() const noexcept {
        return _fullPath.substr(0, _fullPath.find_first_of("?#"));
    }

    constexpr std::string_view subdomain() const noexcept {
        const auto subAndDomain = _host.substr(0, _host.size() - _tld.size() - 1);
        const auto subDomainStart = subAndDomain.rfind('.');
        return subDomainStart != std::string_view::npos ? _host.substr(0, subDomainStart) : std::string_view{};
    }

    constexpr std::string_view name() const noexcept {
        const auto subAndDomain = _host.substr(0, _host.size() - _tld.size() - 1);
        const auto subDomainStart = subAndDomain.rfind('.');
        return subDomainStart != std::string_view::npos ? subAndDomain.substr(subDomainStart + 1) : subAndDomain;
    }

    constexpr std::string_view query() const noexcept {
        const auto queryPos = _fullPath.find_first_of('?');
        if (queryPos == std::string_view::npos) return std::string_view{};
        const auto fragPos = _fullPath.rfind('#');
        return fragPos != std::string_view::npos ? _fullPath.substr(queryPos, fragPos - queryPos)
                                                 : _fullPath.substr(queryPos);
    }

    constexpr std::string_view fragment() const noexcept {
        const auto fragPos = _fullPath.find_first_of('#');
        return fragPos != std::string_view::npos ? _fullPath.substr(fragPos) : std::string_view{};
    }

    constexpr std::pair<std::string_view, std::string_view> auth() const noexcept {
        std::string_view username, password;
        const auto colonPos = _auth.find(':');
        if (colonPos != std::string_view::npos) {
            username = _auth.substr(0, colonPos);
            password = _auth.substr(colonPos + 1);
        }
        return { username, password };
    }

    constexpr std::string_view domain() const noexcept {
        const auto subAndDomain = _host.substr(0, _host.size() - _tld.size() - 1);
        const auto subDomainStart = subAndDomain.rfind('.');
        return subDomainStart != std::string_view::npos ? _host.substr(subDomainStart + 1) : _host;
    };


    static std::string getDomainForURL(const std::string_view& inputURL, const bool includeSubDomain) {
        std::string result;
        try {
            UrlParser p(inputURL.data());
            result.append(includeSubDomain ? p.host() : p.domain());
            if (p.port() != -1) result.append(1, ':').append(std::to_string(p.port()));
        } catch (const std::exception& e) {
            return inputURL.data();
        }
        return result;
    }


private:
    constexpr std::string_view extractScheme(const std::string_view& modURL);
    constexpr std::string_view extractFullPath(const std::string_view& modURL);
    constexpr std::string_view extractAuth(const std::string_view& modURL);
    constexpr std::string_view extractPort(const std::string_view& modURL);
    constexpr std::string_view extractTLD(const std::string_view& modURL);

    const std::string _url;
    std::string_view _scheme;
    std::string_view _auth;
    std::string_view _tld;
    std::string_view _host;
    std::string_view _fullPath;
    int_least32_t _port = -1;
};
