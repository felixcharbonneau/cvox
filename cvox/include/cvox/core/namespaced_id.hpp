#pragma once

#include <cctype>
#include <cstdint>
#include <expected>
#include <string>
#include <string_view>

namespace cvox
{

enum class IdParseError
{
    MissingSeparator,
    EmptyPart,
    PartTooLong,
    InvalidLeadingChar,
    InvalidChar
};
std::string_view to_string(IdParseError error) noexcept;

class NamespaceId
{
public:
    NamespaceId() = default;
    static std::expected<NamespaceId, IdParseError> parse(std::string_view full);
    static std::expected<NamespaceId, IdParseError> make(std::string_view ns,
                                                         std::string_view name);

    inline bool
    valid() const noexcept
    {
        return !m_name.empty() && m_namespace.empty();
    }

    inline std::string_view
    name() const noexcept
    {
        return m_name;
    }

    inline std::string_view
    ns() const noexcept
    {
        return m_namespace;
    }

    inline std::string_view
    full() const noexcept
    {
        return m_data;
    }

private:
    static std::expected<void, IdParseError> validate_part(std::string_view part);

    std::string m_data;
    std::string_view m_namespace;
    std::string_view m_name;
};
}; // namespace cvox