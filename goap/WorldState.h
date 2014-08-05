/**
 * @class WorldState
 * @brief A way of describing the "world" at any point in time.
 *
 * @date  July 2014
 * @copyright (c) 2014 Prylis Inc.. All rights reserved.
 */

#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

struct WorldState {
    float priority_;
    std::string name_;
    std::unordered_map<std::string, bool> vars_;

    WorldState();

    void setVariable(const std::string& name, const bool value);

    /**
     Useful if this state is a goal state. It asks, does state 'other'
     meet the requirements of this goal? Takes into account not only this goal's
     state variables, but which variables matter to this goal state.
     @param other the state you are testing as having met this goal state
     @return true if it meets this goal state, false otherwise
    */
    bool WorldState::meetsGoal(const WorldState& goal_state) const;

    /**
     Given the other state -- and what 'matters' to the other state -- how many
     of our state variables differ from the other?
     @param other the goal state to compare against
     @return the number of state-var differences between us and them
    */
    int distanceTo(const WorldState& goal_state) const;

    bool operator==(const WorldState& other) const;

    // A friend function of a class is defined outside that class' scope but it has the
    // right to access all private and protected members of the class. Even though the
    // prototypes for friend functions appear in the class definition, friends are not
    // member functions.
    friend std::ostream& operator<<(std::ostream& out, const WorldState& n);
};

inline std::ostream& operator<<(std::ostream& out, const WorldState& n) {
    out << "WorldState { ";
    for (const auto& kv : n.vars_) {
        out << kv.first << ":" << kv.second << " | ";
    }
    out << "}";
    return out;
}

