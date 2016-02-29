#ifndef INPUT_H
#define INPUT_H


class Input
{
    public:
        Input();
        virtual ~Input();

        void Update();

        bool arriba;
        bool abajo;
        bool exit;

    protected:

    private:
};

#endif // INPUT_H
