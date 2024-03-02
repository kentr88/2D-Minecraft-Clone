class Block{
public:
    bool liquid;
    float breakTime;

    Block(bool _liquid, float _breakTime){
        liquid = _liquid;
        breakTime = _breakTime;
    }
};
