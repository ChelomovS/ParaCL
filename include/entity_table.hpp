#ifndef ENTITY_TABLE_HPP_
#define ENTITY_TABLE_HPP_

#include <list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stdexcept>

class EntityTable {
  private:
    class EntityScope {
      public:
        std::unordered_map<std::string, int> variables; // NOTE int -> some type (later)

        bool contains(std::string_view name) const {
            return variables.find(std::string(name)) != variables.end();
        }
    };

    std::list<EntityScope> scopes;
  public:
    EntityTable() {
        scopes.emplace_back(); // global scope
    }
    
    void push_scope() {
        scopes.emplace_back();
    }

    void pop_scope() {
        if (scopes.size() > 1) {
            scopes.pop_back();
        } else {
            throw std::runtime_error("Cannot pop global scope");
        }
    }

    bool is_declared(std::string_view name) const {
        for (auto iter_scopes = scopes.rbegin(); iter_scopes != scopes.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                return true;
            }
        }

        return false;
    }

    void declare(std::string_view name, int value) {
        // NOTE maybe throw if variable is declared

        scopes.back().variables.emplace(std::string(name), value);
    }

    void assign(std::string_view name, int value) {
        for (auto iter_scopes = scopes.rbegin(); iter_scopes != scopes.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                iter_scopes->variables[std::string(name)] = value;
                return ;
            }
        }

        throw std::runtime_error("Variable " + std::string(name) + " is not declared");
    }

    int lookup(std::string_view name) const {
        for (auto iter_scopes = scopes.rbegin(); iter_scopes != scopes.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                return iter_scopes->variables.at(std::string(name));
            }
        }

        throw std::runtime_error("Variable " + std::string(name) + "was not declared");   
    }
};

#endif // ENTITY_TABLE_HPP_
