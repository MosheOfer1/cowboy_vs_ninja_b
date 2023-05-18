#include "Cowboy.hpp"

namespace ariel
{
    Cowboy::Cowboy(std::string name, Point location) : Character(name, location, 110), bullets(6) {}

    void Cowboy::shoot(Character *target)
    {
        if (!this->isAlive())
        {
            throw std::runtime_error("Dead characters can't attack!");
        }
        if (!target->isAlive())
        {
            throw std::runtime_error("Can't attack a dead character!");
        }
        if (target == this)
        {
            throw std::runtime_error("One cannot harm himself!");
        }

        if (bullets <= 0)
        {
            if(DEBUG)std::cout << getName() << " is out of bullets!" << std::endl;
        }
        else
        {
            double dist = location.distance(target->getLocation());
        
            if(DEBUG)std::cout << getName() << " shoots " << target->getName() << " and deals " << COWBOY_HARM << " damage!" << std::endl;
            target->hit(COWBOY_HARM);
        
            bullets--;
        }
    }
    bool Cowboy::hasboolets() const
    {
        return bullets > 0;
    }
    void Cowboy::reload()
    {
        if (!isAlive())
        {
            throw std::runtime_error("Dead cowboy...");
        }

        bullets = 6;
    }
    std::string Cowboy::print() const
    {
        std::ostringstream oss;
        oss << "C " << Character::print();
        return oss.str();
    }

}
