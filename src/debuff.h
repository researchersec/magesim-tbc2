namespace debuff
{

    enum ID : int
    {
        SUNDER_ARMOR = 25225,
        EXPOSE_ARMOR = 26866,
        CURSE_OF_RECKLESSNESS = 27226,
        FAERIE_FIRE = 26993,
        BLOOD_FRENZY = 30069,
        MANGLE = 33878,
        GIFT_OF_ARTHAS = 11374,
        IMPROVED_HUNTERS_MARK = 19425,
    };


    class Debuff
    {

    public:
        ID id;
        std::string name;
        double duration;
        int stacks = 1;
        int max_stacks = 1;
        bool hidden = false;

        int addStack()
        {
            if (stacks < max_stacks)
                stacks++;
            else
                return 0;

            return stacks;
        }

    };

    class SunderArmor : public Debuff
    {

    public:
        SunderArmor()
        {
            id = SUNDER_ARMOR;
            name = "Sunder Armor";
            duration = 30;
            max_stacks = 5;
        }

    };

    class ExposeArmor : public Debuff
    {

    public:
        ExposeArmor()
        {
            id = EXPOSE_ARMOR;
            name = "Expose Armor";
            duration = 30;
        }

    };

    class CurseOfRecklessness : public Debuff
    {

    public:
        CurseOfRecklessness()
        {
            id = CURSE_OF_RECKLESSNESS;
            name = "Curse of Recklessness";
            duration = 120;
        }

    };

    class FaerieFire : public Debuff
    {

    public:
        FaerieFire()
        {
            id = FAERIE_FIRE;
            name = "Faerie Fire";
            duration = 40;
        }

    };

    class BloodFrenzy : public Debuff
    {

    public:
        BloodFrenzy()
        {
            id = BLOOD_FRENZY;
            name = "Blood Frenzy";
            duration = 15;
        }

    };

}
