namespace dot
{

    enum ID : int
    {
        DEEP_WOUNDS = 12867,
        REND = 25208,
    };


    class Dot
    {

    public:
        ID id;
        std::string name;
        int ticks = 1;
        int tick = 0;
        double dmg;
        double t_interval = 3;
        bool stackable = false;

        Dot(double _dmg = 0)
        {
            dmg = _dmg;
        }

        virtual void onTick()
        {
            tick++;
        }

        virtual void stack(double _dmg){}

    };

    class DeepWounds : public Dot
    {

    public:
        DeepWounds(double _dmg = 0) : Dot(_dmg)
        {
            id = DEEP_WOUNDS;
            name = "Deep Wounds";
            t_interval = 3;
            ticks = 4;
            stackable = true;
        }

        void stack(double _dmg)
        {
            dmg = _dmg;
            tick = 0;
        }

    };

    class Rend : public Dot
    {

    public:
        Rend(double _dmg = 0) : Dot(_dmg)
        {
            id = REND;
            name = "Rend";
            t_interval = 3;
            ticks = 5;
            dmg = 147;
        }

    };

}
