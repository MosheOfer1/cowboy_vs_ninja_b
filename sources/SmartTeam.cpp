#include "Team.hpp"
#include <algorithm>
#include <limits>

namespace ariel
{
    /**
     * algorithm:
     * cowboy attacks the strongest enemy member that will die in one cowboy shoot, 
     * if not exists he will shoot the enemy member with the lowest AP.
     * If an enemy member in range of 1 meter, Ninja attacks him, 
     * else the ninja will move to the closest enemy member to him  
    */

    void SmartTeam::attack(Team *enemy)
    {
        // Instead of using attackVictim, use smartAttackVictim
        if (enemy == nullptr)
        {
            throw std::invalid_argument("Enemy team can't be null!");
        }
        // Get all members of both teams
        std::vector<Character *> allMembers = getMembers();
        std::vector<Character *> allEnemyMembers = enemy->getMembers();
        std::vector<Character *> aliveMembers = getAliveMembers(allMembers);

        // Check if the attacking team has a living leader
        if (!leader->isAlive())
        {
            Character *closestMember = findClosest(allMembers, leader);
            if (closestMember != nullptr)
            {
                leader = closestMember;
            }
            else
            {
                throw std::runtime_error("no living team members left");
            }
        }

        for (auto attacker : aliveMembers)
        {
            std::vector<Character *> enemyAliveMembers = getAliveMembers(allEnemyMembers);

            smartAttackVictim(attacker, enemyAliveMembers);
        }
    }

    Character *SmartTeam::findWeakestMember(const std::vector<Character *> &members) const
    {
        Character *weakest = nullptr;
        int minAP = std::numeric_limits<int>::max();

        for (auto member : members)
        {
            if (member == nullptr) 
            {
                continue;
            }

            if (member->isAlive() && member->getAttackPoints() < minAP)
            {
                minAP = member->getAttackPoints();
                weakest = member;
            }
        }
        return weakest;
    }

    Character *SmartTeam::findShootableMember(const std::vector<Character *> &members) const
    {
        Character *strongest_so_far = nullptr;
        int current_max_strength = -1;
        for (auto member : members)
        {
            if (member == nullptr) 
            {
                continue;
            }
            if (member->isAlive() && member->getAttackPoints() <= COWBOY_HARM)
            {
                int strength = 0;
                if (dynamic_cast<OldNinja *>(member))
                {
                    strength = 1;
                }
                else if (dynamic_cast<TrainedNinja *>(member))
                {
                    strength = 2;
                }
                else if (dynamic_cast<YoungNinja *>(member))
                {
                    strength = 3;
                }
                else if (dynamic_cast<Cowboy *>(member))
                {
                    strength = 4;
                }

                if (strength > current_max_strength)
                {
                    strongest_so_far = member;
                    current_max_strength = strength;
                }
            }
        }
        return strongest_so_far;
    }

    Character *SmartTeam::findClosestEnemyInRange(const std::vector<Character *> &members, Character *self, double range) const
    {
        Character *closestCharacterInRange = nullptr;
        double closestDist = std::numeric_limits<double>::max();

        for (auto member : members)
        {
            if (member == nullptr) 
            {
                continue;
            }
            double dist = self->distance(member);
            if (member->isAlive() && dist <= range && dist < closestDist)
            {
                closestCharacterInRange = member;
                closestDist = dist;
            }
        }
        return closestCharacterInRange;
    }

    void SmartTeam::smartAttackVictim(Character *attacker, const std::vector<Character *> &enemyAliveMembers)
    {
        if (Cowboy *cowboy = dynamic_cast<Cowboy *>(attacker))
        {
            if (cowboy->hasboolets())
            {
                auto victim = findShootableMember(enemyAliveMembers);
                if (victim == nullptr)
                {
                    victim = findWeakestMember(enemyAliveMembers);
                    if (victim == nullptr)
                    {
                        return;
                    }
                }
                cowboy->shoot(victim);
            }
            else
            {
                cowboy->reload();
            }
        }
        else if (Ninja *ninja = dynamic_cast<Ninja *>(attacker))
        {
            auto victim = findClosestEnemyInRange(enemyAliveMembers, ninja, 1.0);
            if (victim == nullptr)
            {
                victim = findClosest(enemyAliveMembers, ninja);
                if (victim == nullptr)
                {
                    return;
                }
                
                ninja->move(victim);
            }
            else
            {
                ninja->slash(victim);
            }
        }
    }

}
