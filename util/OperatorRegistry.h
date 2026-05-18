#pragma once

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <format>
#include <stdexcept>
#include <type_traits>

namespace ea {

// Compile-time fixed string for use as NTTP
template<size_t N>
struct CFixedString {
    constexpr CFixedString(const char (&str)[N]) noexcept {
        std::copy_n(str, N, value);
    }
    char value[N];
    static constexpr size_t size = N - 1;
    constexpr std::string_view view() const noexcept {
        return std::string_view(value, size);
    }
};

template<size_t N>
CFixedString(const char (&)[N]) -> CFixedString<N>;


// Registry mapping string names to factory functions for a given base type.
// Uses the Meyers singleton to guarantee initialisation order.
template<typename Base>
class AutoRegistry {
public:
    using CreatorFunc = std::unique_ptr<Base>(*)(const std::vector<std::string>&);

    static std::unordered_map<std::string, CreatorFunc>& getCreators() {
        static std::unordered_map<std::string, CreatorFunc> creators_;
        return creators_;
    }

    template<typename Derived>
    static bool registerType(std::string_view name) {
        std::string key(name);
        std::transform(key.begin(), key.end(), key.begin(), ::toupper);
        getCreators()[key] =
            [](const std::vector<std::string>& p) -> std::unique_ptr<Base> {
                if constexpr (std::is_constructible_v<Derived, const std::vector<std::string>&>)
                    return std::make_unique<Derived>(p);
                else
                    return std::make_unique<Derived>();
            };
        return true;
    }

    static std::unique_ptr<Base> create(const std::string& name,
                                        const std::vector<std::string>& params) {
        std::string key = name;
        std::transform(key.begin(), key.end(), key.begin(), ::toupper);
        auto& creators = getCreators();
        auto it = creators.find(key);
        if (it != creators.end())
            return it->second(params);
        throw std::runtime_error(std::format("Unknown operator: '{}'", name));
    }
};


// Intermediate base that handles self-registration in AutoRegistry<RegistryBase>.
// - RegistryBase : the base type used as registry key (e.g. VariationOperator)
// - Derived      : the concrete class being registered (CRTP)
// - TypeName     : compile-time name string used as registry key
// - DirectBase   : the class actually inherited from (defaults to RegistryBase,
//                  set explicitly when an intermediate class exists, e.g. MutationOperator)
template<typename RegistryBase, typename Derived, CFixedString TypeName,
         typename DirectBase = RegistryBase>
class RegisteredExtension : public DirectBase {
private:
    static constexpr auto type_name = TypeName;

    struct RegistrationHelper {
        RegistrationHelper() {
            AutoRegistry<RegistryBase>::template registerType<Derived>(type_name.view());
        }
    };

    static RegistrationHelper& getRegistrationHelper() {
        static RegistrationHelper helper;
        return helper;
    }

    inline static auto& registration_helper_ = getRegistrationHelper();

protected:
    template<typename... Args>
    explicit RegisteredExtension(Args&&... args)
        : DirectBase(std::forward<Args>(args)...) {
        (void)registration_helper_;
    }
};

} // namespace ea
