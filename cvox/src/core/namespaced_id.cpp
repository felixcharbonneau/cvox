#include "cvox/core/namespaced_id.hpp"

namespace cvox
{
std::expected<void, IdParseError>
NamespaceId::validate_part(std::string_view part)
{
    if (part.size() == 0)
    {
        return std::unexpected(IdParseError::EmptyPart);
    }
    auto first = static_cast<unsigned char>(part[0]);
    if (!std::islower(first) && first != '_')
    {
        return std::unexpected(IdParseError::InvalidLeadingChar);
    }
    for (auto c : part)
    {
        unsigned char uc = static_cast<unsigned char>(c);
        if (!std::islower(uc) && !std::isdigit(uc) && uc != '_')
        {
            return std::unexpected(IdParseError::InvalidChar);
        }
    }
    return {};
}

std::expected<NamespaceId, IdParseError>
NamespaceId::make(std::string_view ns, std::string_view name)
{
    const size_t size = ns.size() + name.size() + 1;
    if (size > 64)
    {
        return std::unexpected(IdParseError::PartTooLong);
    }
    if (auto r = validate_part(ns); !r)
    {
        return std::unexpected(r.error());
    }
    if (auto r = validate_part(name); !r)
    {
        return std::unexpected(r.error());
    }

    NamespaceId out;
    out.m_data.reserve(size);
    out.m_data.append(ns).append(":").append(name);
    out.m_namespace = out.m_data.substr(0, ns.size());
    out.m_name = out.m_data.substr(ns.size() + 1);

    return out;
}

std::expected<NamespaceId, IdParseError>
NamespaceId::parse(std::string_view str)
{
    const auto separator = str.find(":");
    if (separator == std::string_view::npos)
    {
        return std::unexpected(IdParseError::MissingSeparator);
    }
    return make(str.substr(0, separator), str.substr(separator + 2));
}

std::string_view
to_string(IdParseError err) noexcept
{
    switch (err)
    {
    case IdParseError::MissingSeparator:
        return "missing \':\' separator";
    case IdParseError::EmptyPart:
        return "namespace or name part is empty";
    case IdParseError::PartTooLong:
        return "part exceeds the limit of 64 characters";
    case IdParseError::InvalidLeadingChar:
        return "part must start with a lowercase letter or underscore";
    case IdParseError::InvalidChar:
        return "part may only contain lowercase letters, digits and underscores";
    }
    return "unknown error";
}
} // namespace cvox