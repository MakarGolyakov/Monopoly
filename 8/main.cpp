#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <ctime>
#include <random>
#include <algorithm>

using std::cin;
using std::vector;
using std::string;
using std::cout;
using std::endl;

using namespace sf;

struct street {
    string title;
    string color;
    int homeCount = 0;
    int position;
    bool hotelCount = 0;
    string owner = "���";
    int cost;
    int rent = 0;
    /*
    street() {
        title = "title";
        color = "color";
        position = 1;
        owner = "no";
        cost = 320;
    }
    */
    street(const string name, const string colour, int pos, int money) {
        title = name;
        color = colour;
        position = pos;
        cost = money;
    }

};

struct color {
private:
    vector<street> streets;
    bool singleOwner;
};

struct player {
    string name;
    int cash = 50;
    int currentPosition = 0;
    vector<street> srteets;
    bool inJail = false;
    bool freeJail = false;
    player(const string& n) {
        name = n;
    }
};

bool clicked(RectangleShape button, Vector2i click, int butW, int butH) {
    int x = button.getPosition().x;
    int y = button.getPosition().y;
    int clickX = click.x;
    int clickY = click.y;
    if (clickX >= x && clickX <= x + butW && clickY >= y && clickY <= y + butH) {
        return true;
    }
    return false;
}

enum windowState {
    START,
    START_INPUT,
    IN_GAME,
    CUBE_DROPED,
    GAMER_STEP,
    TALE_MARK,
    TALE_PREWIEW,
    TAX_100,
    TAX_200,
    COMMONU_CHEST,
    CHANCE,
    TO_JAIL_TABLE,
    POINT_THE_TILE,
    GAME_OVER,
    EXIT,
};

enum figures {
    DOG,
    CAT,
    CAR,
    HAT,
    BOOT,
    SHIP,
};

int currentWindowState = START;

vector<string> chance{
        "������ ����� ������������ ������� ����� �������������. ��������� �� ������ ��� �� 25, �� ������ ����� �� 100",
        "������������� �� St.James Place",
        "������������� �� Boardwalk",
        "������������� �� ���� Start",
        "������������� � ������",
        "�������� ���� ���������� ������� �� ����� ����� �� �������������. �������� 150",
        "��������� �� 3 ���� �����",
        "���� ������ ��� ��������� � ������� 50",
        "������������� �� Reading railroad",
        "������������� �� Virginia Avenue",
        "����� �� ���������� ��������. ��������� 15",
        "��� ������� ������������� ������ ����������. ��������� ������� ������ �� 50",
        "������������� �� ��������� �/� �������. ���� ��� � �������������, ��������� ��������� �������� �����",
        "������������� �� ��������� ������������ �����������. ���� ��� ��������� � �������������, ������������� ����� �������� �����",
        "���������� ������������ �� ������"
};

vector<int> usedFigures;

bool checkUsedFigures(int figure) {
    for (int i = 0; i < usedFigures.size(); ++i) {
        if (usedFigures[i] == figure) {
            return 1;
        }
    }
    return 0;
}

bool checkLose(player& a) {
    if (a.cash < 0) {
        return true;
    }
    return false;
}

vector<string> communityChest{
       "�� ��� ��������� �������� ��������� ������. �������� �� ������ ��� �� 40, �� ������ ����� �� 115",
       "�������� ������� �� �������������� � ������� 100",
       "����� �� ���� Start",
       "�� ������ ������ ����� �� �������� �������. �������� 10",
       "������� ��� ���� ��������. �������� �� 10 �� ������� ������",
       "�������� �������� � ������� 50",
       "�������� 25 �� �������������� ������",
       "�������� ���� ���������� ������� �� ����������� �����. �������� 100",
       "���������� ����������� ������. �������� 20",
       "����� � �����. ��������� 50",
       "���������� ������ � ���� ������. �������� 200",
       "���������� ������������ �� ������",
       "������������� � ������",
       "�� ������� ����� �� ���������� 50",
       "�������� ���� ���������� ������� �� ���������� �����. �������� 100",
       "�� ��������� � ���������� 100"
};

vector<player> players;

vector<street> streets = {
   street("Start", "-", 0, 0),
   street("Mediterranean Avenue", "����������", 1, 60),
   street("CC", "-", 2, 0),
   street("Baltic Avenue", "����������", 3, 60),
   street("Income Tax", "-", 4, 200),
   street("Reading RailRoad", "�/�", 5, 200),
   street("Oriental Avenue", "�������", 6, 100),
   street("C", "-", 7, 0),
   street("Vermont Avenue", "�������", 8, 100),
   street("Connecticut Avenue", "�������", 9, 120),
   street("Jail", "-", 10, 0),
   street("St. Sharles Place", "�������", 11, 140),
   street("Ellectic Company", "������������", 12, 150),
   street("States Avenue", "�������", 13, 140),
   street("Virginia Avenue", "�������", 14, 160),
   street("Pennsylvania RailRoad", "�/�", 15, 200),
   street("St. James Place", "���������", 16, 180),
   street("CC", "-", 17, 0),
   street("Tennessee Avenue", "���������", 18, 180),
   street("New York Avenue", "���������", 19, 200),
   street("Parking", "-", 20, 0),
   street("Kentucky Avenue", "�������", 21, 220),
   street("C", "-", 22, 0),
   street("Indiana Avenue", "�������", 23, 220),
   street("Illinois Avenue", "�������", 24, 240),
   street("B. & O. RailRoad", "�/�", 25, 200),
   street("Atlantic Avenue", "������", 26, 260),
   street("Ventnor Avenue", "������", 27, 260),
   street("Water Works", "������������", 28, 150),
   street("Marvin Avenue", "������", 29, 280),
   street("Go Jail", "-", 30, 0),
   street("Pacific Avenue", "�������", 31, 300),
   street("North Carolina Avenue", "�������", 32, 300),
   street("CC", "-", 33, 0),
   street("Pennsylvania Avenue", "�������", 34, 320),
   street("Short Line", "�/�", 35, 200),
   street("C", "-", 36, 0),
   street("Park Place", "�����", 37, 350),
   street("Luxury Tax", "-", 38, 100),
   street("BoardWalk", "�����", 39, 400),
};

vector<RectangleShape> tales;

int playerQ = 2;

int currentGamer = 0;

int cubeResult = 0;

void drawInfo(RenderWindow& win, Font& font, street str) {
    Text info("", font, 30);
    info.setFillColor(Color::White);
    info.setStyle(Text::Bold);
    info.setString("��������: " + str.title);
    info.setPosition(250, 350);
    win.draw(info);
    info.setString("����(�������): " + str.color);
    info.setPosition(250, 400);
    win.draw(info);
    info.setString("����: " + std::to_string(str.cost));
    info.setPosition(250, 450);
    win.draw(info);
    info.setString("��������: " + str.owner);
    info.setPosition(250, 500);
    win.draw(info);

}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(950, 950), "Monopoly");
    Font font;
    font.loadFromFile("E:/������� ����/8/CyrilicOld.TTF");

    RectangleShape tale0(Vector2f(125, 125));
    tale0.setPosition(825, 825);
    tale0.setFillColor(Color(0, 0, 0, 0));
    tales.push_back(tale0);
    window.draw(tale0);
    int shift = 76;
    for (int i = 0; i < 9; ++i) {
        RectangleShape tale(Vector2f(76, 125));
        tale.setPosition(825 - shift, 825);
        tale.setFillColor(Color(0, 0, 0, 0));
        tales.push_back(tale);
        window.draw(tale);
        shift += 78;
    }
    RectangleShape tale10(Vector2f(125, 125));
    tale10.setPosition(0, 825);
    tale10.setFillColor(Color(0, 0, 0, 1));
    tales.push_back(tale10);
    window.draw(tale10);
    shift = 76;
    for (int i = 0; i < 9; ++i) {
        RectangleShape tale(Vector2f(125, 76));
        tale.setPosition(0, 825 - shift);
        tale.setFillColor(Color(0, 0, 0, 0));
        tales.push_back(tale);
        window.draw(tale);
        shift += 78;
    }
    RectangleShape tale20(Vector2f(125, 125));
    tale20.setPosition(0, 0);
    tale20.setFillColor(Color(0, 0, 0, 0));
    tales.push_back(tale20);
    window.draw(tale20);
    shift = 125;
    for (int i = 0; i < 9; ++i) {
        RectangleShape tale(Vector2f(76, 125));
        tale.setPosition(0 + shift, 0);
        tale.setFillColor(Color(0, 0, 0, 0));
        tales.push_back(tale);
        window.draw(tale);
        shift += 78;
    }
    RectangleShape tale40(Vector2f(125, 125));
    tale40.setPosition(825, 0);
    tale40.setFillColor(Color(0, 0, 0, 0));
    tales.push_back(tale40);
    window.draw(tale40);
    shift = 125;
    for (int i = 0; i < 9; ++i) {
        RectangleShape tale(Vector2f(125, 76));
        tale.setPosition(825, 0 + shift);
        tale.setFillColor(Color(0, 0, 0, 0));
        tales.push_back(tale);
        window.draw(tale);
        shift += 78;
    }
    //window.display();

    while (window.isOpen()) {
        switch (currentWindowState)
        {
        case START:
        {
            Image bluredField;
            bluredField.loadFromFile("E:/������� ����/8/bluredField.jpg");

            Texture bluredFieldTexture;
            bluredFieldTexture.loadFromImage(bluredField);

            Sprite bluredSprite(bluredFieldTexture);
            bluredSprite.setPosition(0, 0);

            window.clear();
            window.draw(bluredSprite);
            //Font font;
            //font.loadFromFile("E:/������� ����/8/CyrilicOld.TTF");
            Text playerSet("", font, 50);
            playerSet.setFillColor(Color::White);
            playerSet.setStyle(Text::Bold);
            playerSet.setString("������� ����� �������?");
            playerSet.setPosition(200, 10);
            window.draw(playerSet);
            RectangleShape button1(Vector2f(100, 50));
            RectangleShape button2(Vector2f(100, 50));
            RectangleShape button3(Vector2f(100, 50));
            RectangleShape button4(Vector2f(100, 50));
            RectangleShape button5(Vector2f(100, 50));
            RectangleShape button6(Vector2f(100, 50));
            button1.setPosition(50, 100);
            button2.setPosition(200, 100);
            button3.setPosition(350, 100);
            button4.setPosition(500, 100);
            button5.setPosition(650, 100);
            button6.setPosition(800, 100);
            window.draw(button1);
            window.draw(button2);
            window.draw(button3);
            window.draw(button4);
            window.draw(button5);
            window.draw(button6);
            int beforePos = 87;
            for (int i = 0; i < 6; ++i) {
                Text numbers("", font, 50);
                numbers.setFillColor(Color::Black);
                numbers.setStyle(Text::Bold);
                numbers.setString(char(i + 49));
                numbers.setPosition(beforePos, 93);
                beforePos += 150;
                window.draw(numbers);
            }

            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:

                    if (event.mouseButton.button == sf::Mouse::Left && clicked(button1, Mouse::getPosition(window), 100, 50)) {
                        playerQ = 1;
                        currentWindowState = START_INPUT;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left && clicked(button2, Mouse::getPosition(window), 100, 50)) {
                        playerQ = 2;
                        currentWindowState = START_INPUT;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left && clicked(button3, Mouse::getPosition(window), 100, 50)) {
                        playerQ = 3;
                        currentWindowState = START_INPUT;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left && clicked(button4, Mouse::getPosition(window), 100, 50)) {
                        playerQ = 4;
                        currentWindowState = START_INPUT;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left && clicked(button5, Mouse::getPosition(window), 100, 50)) {
                        playerQ = 5;
                        currentWindowState = START_INPUT;
                    }
                    if (event.mouseButton.button == sf::Mouse::Left && clicked(button6, Mouse::getPosition(window), 100, 50)) {
                        playerQ = 6;
                        currentWindowState = START_INPUT;
                    }
                }
            }
            window.display();
            break;
        }

        case START_INPUT:
        {
            RectangleShape figure1(Vector2f(100, 50)); //������
            RectangleShape figure2(Vector2f(100, 50)); //���
            RectangleShape figure3(Vector2f(100, 50)); //�������
            RectangleShape figure4(Vector2f(100, 50)); //�����
            RectangleShape figure5(Vector2f(100, 50)); //�����
            RectangleShape figure6(Vector2f(100, 50)); //��������
            figure1.setPosition(50, 500);
            figure2.setPosition(200, 500);
            figure3.setPosition(350, 500);
            figure4.setPosition(500, 500);
            figure5.setPosition(650, 500);
            figure6.setPosition(800, 500);
            window.draw(figure1);
            window.draw(figure2);
            window.draw(figure3);
            window.draw(figure4);
            window.draw(figure5);
            window.draw(figure6);
            window.display();
            while (players.size() != playerQ) {
                Event event;
                while (window.pollEvent(event)) {
                    switch (event.type) {
                    case Event::Closed:
                        window.close();
                        break;

                    case Event::MouseButtonPressed:
                        if (event.mouseButton.button == sf::Mouse::Left && clicked(figure1, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(0)) {
                                usedFigures.push_back(DOG);
                                player p("������");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("����� " + char(players.size() + 48));
                                gamer.setPosition(figure1.getPosition().x, figure1.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();

                            }
                        }
                        if (event.mouseButton.button == Mouse::Left && clicked(figure2, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(1)) {
                                usedFigures.push_back(CAT);
                                player p("���");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("����� " + char(players.size() + 48));
                                gamer.setPosition(figure2.getPosition().x, figure2.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }

                        }
                        if (event.mouseButton.button == Mouse::Left && clicked(figure3, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(2)) {
                                usedFigures.push_back(CAR);
                                player p("�������");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("����� " + char(players.size() + 48));
                                gamer.setPosition(figure3.getPosition().x, figure3.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }
                        }
                        if (event.mouseButton.button == sf::Mouse::Left && clicked(figure4, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(3)) {
                                usedFigures.push_back(HAT);
                                player p("�����");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("����� " + char(players.size() + 48));
                                gamer.setPosition(figure4.getPosition().x, figure4.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }
                        }
                        if (event.mouseButton.button == sf::Mouse::Left && clicked(figure5, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(4)) {
                                usedFigures.push_back(BOOT);
                                player p("�����");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("����� " + char(players.size() + 48));
                                gamer.setPosition(figure5.getPosition().x, figure5.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }
                        }
                        if (event.mouseButton.button == sf::Mouse::Left && clicked(figure6, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(5)) {
                                usedFigures.push_back(SHIP);
                                player p("��������");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("����� " + char(players.size() + 48));
                                gamer.setPosition(figure6.getPosition().x, figure6.getPosition().y + 70);
                                window.draw(gamer);
                            }
                        }
                    }
                }
            }
            currentWindowState = IN_GAME;
            break;
        }
        case IN_GAME:
        {

            Image normalField;
            normalField.loadFromFile("E:/������� ����/8/bestField3.jpg");

            Texture normalFieldTexture;
            normalFieldTexture.loadFromImage(normalField);

            Sprite normalSprite(normalFieldTexture);
            normalSprite.setPosition(0, 0);

            window.clear();
            window.draw(normalSprite);

            RectangleShape dropCube(Vector2f(170, 70));
            dropCube.setPosition(650, 150);
            dropCube.setFillColor(Color::Green);

            window.draw(dropCube);

            Text cost("", font, 20);
            cost.setFillColor(Color::Black);
            cost.setStyle(Text::Bold);
            cost.setString("����: " + std::to_string(players[currentGamer].cash));
            cost.setPosition(350, 130);
            window.draw(cost);

            Text drop("", font, 25);
            drop.setFillColor(Color::Black);
            drop.setStyle(Text::Bold);
            drop.setString("������� �����");
            drop.setPosition(dropCube.getPosition().x, dropCube.getPosition().y + 20);
            window.draw(drop);

            Text gamerTilt("", font, 20);
            gamerTilt.setFillColor(Color::Black);
            gamerTilt.setStyle(Text::Bold);
            gamerTilt.setString("�����: " + players[currentGamer].name);
            gamerTilt.setPosition(140, 130);
            window.draw(gamerTilt);
            window.display();
            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && clicked(dropCube, Mouse::getPosition(window), 170, 70)) {
                        int result = rand() % 6;
                        Text res("", font, 25);
                        currentWindowState = CUBE_DROPED;
                    }
                    break;
                }

            }

            if (players.size() == 1) {
                currentWindowState = EXIT;
            }


            break;
        }
        case CUBE_DROPED:
        {
            if (!cubeResult) {
                cubeResult = rand() % 12 + 1;
            }
            Text res("", font, 40);
            res.setFillColor(Color::Black);
            res.setStyle(Text::Bold);
            res.setString(std::to_string(cubeResult));
            res.setPosition(720, 230);
            window.draw(res);

            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                }
            }
            currentWindowState = GAMER_STEP;
            window.display();
            break;
        }
        case GAMER_STEP:
        {
            for (int i = 0; i < cubeResult; ++i) {
                players[currentGamer].currentPosition = (players[currentGamer].currentPosition + 1) % 40;;
                if (players[currentGamer].currentPosition == 0) {
                    players[currentGamer].cash += 200;
                }
            }
            if (players[currentGamer].currentPosition == 30) {
                players[currentGamer].inJail = true;
                players[currentGamer].currentPosition = 10;
                currentWindowState = TO_JAIL_TABLE;
            }
            else if (players[currentGamer].currentPosition == 7 || players[currentGamer].currentPosition == 22 || players[currentGamer].currentPosition == 36) {
                //����������� �������� ����

                currentWindowState = TALE_MARK;
            }
            else if (players[currentGamer].currentPosition == 2 || players[currentGamer].currentPosition == 17 || players[currentGamer].currentPosition == 33) {
                //����������� �������� ������������ �����
                currentWindowState = TALE_MARK;
            }
            else if (players[currentGamer].currentPosition == 4) {
                currentWindowState = TAX_200;
            }
            else if (players[currentGamer].currentPosition == 38) {
                currentWindowState = TAX_100;
            }
            else if (players[currentGamer].currentPosition == 0 || players[currentGamer].currentPosition == 10 || players[currentGamer].currentPosition == 20) {

                currentGamer = (currentGamer + 1) % playerQ;
                currentWindowState = POINT_THE_TILE;
            }
            else {
                currentWindowState = TALE_MARK;
            }
            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                }
            }
            //window.display();
            break;
        }
        case POINT_THE_TILE:
        {
            tales[players[currentGamer].currentPosition].setFillColor(Color(0, 0, 0, 1));
            window.draw(tales[players[currentGamer].currentPosition]);
            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    currentWindowState = IN_GAME;
                }
            }
            window.display();
            break;
        }
        case TO_JAIL_TABLE:
        {

        }
        }
        return 0;
    }