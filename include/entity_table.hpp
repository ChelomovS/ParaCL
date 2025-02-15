#ifndef ENTITY_TABLE_HPP
#define ENTITY_TABLE_HPP

#include <list>
#include <string>
#include <unordered_map>
#include <stdexcept>
#include <cassert>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ranges.h>

class EntityTable {
  public:
    using EntityScope = std::unordered_map<std::string, int>;
  private:
    std::list<EntityScope> scopes_;

  public:
    EntityTable() {
        scopes_.emplace_back(); // global scope
    }
    
    void push_scope(EntityScope scope = EntityScope{}) {
        scopes_.emplace_back(scope);
        spdlog::trace("add scope on top");
    }

    void pop_scope() {
        if (scopes_.size() > 1) {
            scopes_.pop_back();
        } else {
            throw std::runtime_error("Cannot pop global scope");
        }
        spdlog::trace("remove scope from top");
    }

    EntityScope top_scope() {
        spdlog::trace("obtain top scope");
        return scopes_.back();
    }

    // checks is variable declared at all
    bool is_declared_global(std::string name) const { 
        for (auto iter_scopes = scopes_.rbegin(); iter_scopes != scopes_.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                return true;
            }
        }

        return false;
    }

    // checks is variable declared in currenct scope
    bool is_declared_scope(std::string name) const { 
        return scopes_.back().contains(name);
    }

    void declare(std::string name, int value) {
        // NOTE maybe throw if variable is declared

        assert(!scopes_.empty());
        
        scopes_.back().emplace(std::string(name), value);
    }

    void assign(std::string name, int value) {
        for (auto iter_scopes = scopes_.rbegin(); iter_scopes != scopes_.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                iter_scopes->at(std::string(name)) = value;
                return ;
            }
        }

        throw std::runtime_error("Variable " + std::string(name) + " is not declared");
    }

    int lookup(std::string name) const {
        for (auto iter_scopes = scopes_.rbegin(); iter_scopes != scopes_.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                return iter_scopes->at(std::string(name));
            }
        }

        throw std::runtime_error("Variable " + std::string(name) + "was not declared");   
    }

    void log() {
        spdlog::trace("scopes: {}", scopes_);
    }
};

#endif // ENTITY_TABLE_HPP
