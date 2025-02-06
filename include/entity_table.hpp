<<<<<<< HEAD
#ifndef ENTITY_TABLE_HPP
#define ENTITY_TABLE_HPP
=======
#ifndef ENTITY_TABLE_HPP_
#define ENTITY_TABLE_HPP_
>>>>>>> ecf2ab55ae32a6f456610953c12dadea2f4e7259

#include <list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <stdexcept>
<<<<<<< HEAD
#include <iterator>
#include <cassert>
=======
>>>>>>> ecf2ab55ae32a6f456610953c12dadea2f4e7259

class EntityTable {
  private:
    class EntityScope {
      public:
<<<<<<< HEAD
        std::unordered_map<std::string, int> variables;

        bool contains(std::string name) const {
            return variables.find(name) != variables.end();
        }
    };

    std::list<EntityScope> scopes; 

=======
        std::unordered_map<std::string, int> variables; // NOTE int -> some type (later)

        bool contains(std::string_view name) const {
            return variables.find(std::string(name)) != variables.end();
        }
    };

    std::list<EntityScope> scopes;
>>>>>>> ecf2ab55ae32a6f456610953c12dadea2f4e7259
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

<<<<<<< HEAD
    bool is_declared(std::string name) const {
=======
    bool is_declared(std::string_view name) const {
>>>>>>> ecf2ab55ae32a6f456610953c12dadea2f4e7259
        for (auto iter_scopes = scopes.rbegin(); iter_scopes != scopes.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                return true;
            }
        }

        return false;
    }

<<<<<<< HEAD
    void declare(std::string name, int value) {
        // NOTE maybe throw if variable is declared

        assert(!scopes.empty());
        
        scopes.back().variables.emplace(std::string(name), value);
    }

    void assign(std::string name, int value) {
=======
    void declare(std::string_view name, int value) {
        // NOTE maybe throw if variable is declared

        scopes.back().variables.emplace(std::string(name), value);
    }

    void assign(std::string_view name, int value) {
>>>>>>> ecf2ab55ae32a6f456610953c12dadea2f4e7259
        for (auto iter_scopes = scopes.rbegin(); iter_scopes != scopes.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                iter_scopes->variables[std::string(name)] = value;
                return ;
            }
        }

        throw std::runtime_error("Variable " + std::string(name) + " is not declared");
    }

<<<<<<< HEAD
    int lookup(std::string name) const {
=======
    int lookup(std::string_view name) const {
>>>>>>> ecf2ab55ae32a6f456610953c12dadea2f4e7259
        for (auto iter_scopes = scopes.rbegin(); iter_scopes != scopes.rend(); ++iter_scopes) {
            if (iter_scopes->contains(name)) {
                return iter_scopes->variables.at(std::string(name));
            }
        }

        throw std::runtime_error("Variable " + std::string(name) + "was not declared");   
    }
};

<<<<<<< HEAD
#endif // ENTITY_TABLE_HPP
=======
#endif // ENTITY_TABLE_HPP_
>>>>>>> ecf2ab55ae32a6f456610953c12dadea2f4e7259
