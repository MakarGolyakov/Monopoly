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
    string owner = "Нет";
    int cost;
    int rent;

    street(const string name, const string colour, int pos, int money, int nrent) {
        title = name;
        color = colour;
        position = pos;
        cost = money;
        rent = nrent;
    }

};

struct color {
private:
    vector<street> streets;
    bool singleOwner;
};

struct player {
    string name;
    int cash = 100;
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
        "Отправляйтесь на поле Start",
        "Отправляйтесь в тюрьму",
        "Наступил срок исполнения\nплатежа по вашей ссуде\nна строительство. Получите 150",
        "Банк платит вам дивиденты в размере 50",
        "Штраф за превышение скорости.\nЗаплатите 15",
        "Вас избрали председателем\nсовета директоров. Заплатите\nкаждому игроку по 50",
        "Бесплатное освобождение из тюрьмы"
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
       "Оплатите расходы на\nгоспитализацию в размере\n100",
       "Оплатите обучение\nв размере 50",
       "Наступил срок исполнения\nплатежа по страхованию\nжизни. Получите 100",
       "Визит к врачу.\nЗаплатите 50",
       "Банковская ошибка в\nвашу пользу. Получите 200",
       "Бесплатное освобождение \nиз тюрьмы",
       "Наступил срок исполнения\nплатежа по отпускному\nфонду. Получите 100",
};

vector<player> players;

vector<street> streets = {
   street("Start", "-", 0, 0, 0),
   street("Mediterranean Avenue", "Коричневый", 1, 60, 12),
   street("CC", "-", 2, 0, 0),
   street("Baltic Avenue", "Коричневый", 3, 60, 12),
   street("Income Tax", "-", 4, 200, 0),
   street("Reading RailRoad", "ж/д", 5, 200, 100),
   street("Oriental Avenue", "Голубой", 6, 100, 30),
   street("C", "-", 7, 0, 0),
   street("Vermont Avenue", "Голубой", 8, 100, 33),
   street("Connecticut Avenue", "Голубой", 9, 120, 37),
   street("Jail", "-", 10, 0, 0),
   street("St. Sharles Place", "Розовый", 11, 140, 45),
   street("Ellectic Company", "Коммунальные", 12, 150, 100),
   street("States Avenue", "Розовый", 13, 140, 46),
   street("Virginia Avenue", "Розовый", 14, 160, 47),
   street("Pennsylvania RailRoad", "ж/д", 15, 200, 100),
   street("St. James Place", "Оранжевый", 16, 180, 68),
   street("CC", "-", 17, 0, 0),
   street("Tennessee Avenue", "Оранжевый", 18, 180, 75),
   street("New York Avenue", "Оранжевый", 19, 200, 90),
   street("Parking", "-", 20, 0, 0),
   street("Kentucky Avenue", "Красный", 21, 220, 100),
   street("C", "-", 22, 0, 0),
   street("Indiana Avenue", "Красный", 23, 220, 110),
   street("Illinois Avenue", "Красный", 24, 240, 125),
   street("B. & O. RailRoad", "ж/д", 25, 200, 100),
   street("Atlantic Avenue", "Желтый", 26, 260, 170),
   street("Ventnor Avenue", "Желтый", 27, 260, 190),
   street("Water Works", "Коммунальные", 28, 150, 100),
   street("Marvin Avenue", "Желтый", 29, 280, 200),
   street("Go Jail", "-", 30, 0, 0),
   street("Pacific Avenue", "Зеленый", 31, 300, 230),
   street("North Carolina Avenue", "Зеленый", 32, 300, 270),
   street("CC", "-", 33, 0, 0),
   street("Pennsylvania Avenue", "Зеленый", 34, 320, 330),
   street("Short Line", "ж/д", 35, 200, 100),
   street("C", "-", 36, 0, 0),
   street("Park Place", "Синий", 37, 350, 350),
   street("Luxury Tax", "-", 38, 100, 0),
   street("BoardWalk", "Синий", 39, 400, 400),
};

vector<RectangleShape> tales;

int playerQ = 2;

int currentGamer = 0;

int cubeResult = 0;

bool pressed = false;

void drawInfo(RenderWindow& win, Font& font, street str) {
    Text info("", font, 30);
    info.setFillColor(Color::White);
    info.setStyle(Text::Bold);
    info.setString("Название: " + str.title);
    info.setPosition(250, 350);
    win.draw(info);
    info.setString("Цвет(отрасль): " + str.color);
    info.setPosition(250, 400);
    win.draw(info);
    info.setString("Цена: " + std::to_string(str.cost));
    info.setPosition(250, 450);
    win.draw(info);
    info.setString("Владелец: " + str.owner);
    info.setPosition(250, 500);
    win.draw(info);
}

void drawChance(RenderWindow& window, Font& font, bool type, int number) {
    bool submit = false;
    while (!submit) {
        tales[players[currentGamer].currentPosition].setFillColor(Color(0, 0, 0, 1));
        window.draw(tales[players[currentGamer].currentPosition]);
        RectangleShape InfoWindow(Vector2f(500, 300));
        InfoWindow.setPosition(225, 325);
        InfoWindow.setFillColor(Color(0, 0, 0, 3));
        window.draw(InfoWindow);
        Text tax100("", font, 20);
        tax100.setFillColor(Color::White);
        if (type == 0) {
            tax100.setString(chance[number]);
        }
        else {
            tax100.setString(communityChest[number]);
        }
        tax100.setPosition(300, 400);
        window.draw(tax100);

        RectangleShape BUY(Vector2f(120, 50));
        BUY.setPosition(420, 550);
        BUY.setFillColor(Color::White);
        window.draw(BUY);

        Text buy("", font, 25);
        buy.setFillColor(Color::Black);
        buy.setStyle(Text::Bold);
        buy.setString("ОК");
        buy.setPosition(459, 557);
        window.draw(buy);

        Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case Event::Closed:
                window.close();
                break;

            case Event::MouseButtonPressed:
                if (event.mouseButton.button == Mouse::Left && clicked(BUY, Mouse::getPosition(window), 120, 50)) {
                    //currentWindowState = IN_GAME;
                    //cubeResult = 0;
                    submit = true;
                }
            }
        }
        window.display();
    }

}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(950, 950), "Monopoly");
    Font font;
    font.loadFromFile("E:/Рабочий стол/8/CyrilicOld.TTF");

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
            bluredField.loadFromFile("E:/Рабочий стол/8/bluredField.jpg");

            Texture bluredFieldTexture;
            bluredFieldTexture.loadFromImage(bluredField);

            Sprite bluredSprite(bluredFieldTexture);
            bluredSprite.setPosition(0, 0);

            window.clear();
            window.draw(bluredSprite);
            //Font font;
            //font.loadFromFile("E:/Рабочий стол/8/CyrilicOld.TTF");
            Text playerSet("", font, 50);
            playerSet.setFillColor(Color::White);
            playerSet.setStyle(Text::Bold);
            playerSet.setString("Сколько будет игроков?");
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
            RectangleShape figure1(Vector2f(100, 50)); //Собака
            RectangleShape figure2(Vector2f(100, 50)); //Кот
            RectangleShape figure3(Vector2f(100, 50)); //Машинка
            RectangleShape figure4(Vector2f(100, 50)); //Шляпа
            RectangleShape figure5(Vector2f(100, 50)); //Сапог
            RectangleShape figure6(Vector2f(100, 50)); //Кораблик
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
            Text text("", font, 20);
            Text cat("", font, 50);
            Text car("", font, 50);
            Text hat("", font, 50);
            Text boot("", font, 50);
            Text ship("", font, 50);

            text.setFillColor(Color::Black);
            text.setString("Собака");
            text.setPosition(71, 510);
            window.draw(text);

            text.setString("Кот");
            text.setPosition(233, 510);
            window.draw(text);

            text.setString("Машинка");
            text.setPosition(357, 510);
            window.draw(text);

            text.setString("Шляпа");
            text.setPosition(517, 510);
            window.draw(text);

            text.setString("Сапог");
            text.setPosition(677, 510);
            window.draw(text);

            text.setString("Кораблик");
            text.setPosition(810, 510);
            window.draw(text);

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
                                player p("Собака");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("Игрок " + char(players.size() + 48));
                                gamer.setPosition(figure1.getPosition().x, figure1.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();

                            }
                        }
                        if (event.mouseButton.button == Mouse::Left && clicked(figure2, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(1)) {
                                usedFigures.push_back(CAT);
                                player p("Кот");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("Игрок " + char(players.size() + 48));
                                gamer.setPosition(figure2.getPosition().x, figure2.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }

                        }
                        if (event.mouseButton.button == Mouse::Left && clicked(figure3, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(2)) {
                                usedFigures.push_back(CAR);
                                player p("Машинка");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("Игрок " + char(players.size() + 48));
                                gamer.setPosition(figure3.getPosition().x, figure3.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }
                        }
                        if (event.mouseButton.button == sf::Mouse::Left && clicked(figure4, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(3)) {
                                usedFigures.push_back(HAT);
                                player p("Шляпа");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("Игрок " + char(players.size() + 48));
                                gamer.setPosition(figure4.getPosition().x, figure4.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }
                        }
                        if (event.mouseButton.button == sf::Mouse::Left && clicked(figure5, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(4)) {
                                usedFigures.push_back(BOOT);
                                player p("Сапог");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("Игрок " + char(players.size() + 48));
                                gamer.setPosition(figure5.getPosition().x, figure5.getPosition().y + 70);
                                window.draw(gamer);
                                //window.display();
                            }
                        }
                        if (event.mouseButton.button == sf::Mouse::Left && clicked(figure6, Mouse::getPosition(window), 100, 50)) {
                            if (!checkUsedFigures(5)) {
                                usedFigures.push_back(SHIP);
                                player p("Кораблик");
                                players.push_back(p);
                                Text gamer("", font, 20);
                                gamer.setFillColor(Color::Red);
                                gamer.setStyle(Text::Bold);
                                gamer.setString("Игрок " + char(players.size() + 48));
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
            normalField.loadFromFile("E:/Рабочий стол/8/bestField3.jpg");

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
            cost.setString("Счет: " + std::to_string(players[currentGamer].cash));
            cost.setPosition(350, 130);
            window.draw(cost);

            Text drop("", font, 25);
            drop.setFillColor(Color::Black);
            drop.setStyle(Text::Bold);
            drop.setString("Бросить кубик");
            drop.setPosition(dropCube.getPosition().x, dropCube.getPosition().y + 20);
            window.draw(drop);

            Text gamerTilt("", font, 20);
            gamerTilt.setFillColor(Color::Black);
            gamerTilt.setStyle(Text::Bold);
            gamerTilt.setString("Игрок: " + players[currentGamer].name);
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
                //cubeResult = rand() % 12 + 1;
                cubeResult = 1;
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

            if (players[currentGamer].inJail == true) {
                currentGamer = (currentGamer + 1) % playerQ;
                players[currentGamer].inJail == false;
            }

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
                int randChance = rand() % 7;
                if (randChance == 0) {
                    drawChance(window, font, 0, randChance);
                    players[currentGamer].currentPosition = 0;
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                    players[currentGamer].cash += 200;
                }
                if (randChance == 1) {
                    drawChance(window, font, 0, randChance);
                    players[currentGamer].currentPosition = 10;
                    players[currentGamer].inJail = true;
                    currentWindowState = TO_JAIL_TABLE;
                }
                if (randChance == 2) {
                    drawChance(window, font, 0, randChance);
                    players[currentGamer].cash += 150;
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                }
                if (randChance == 3) {
                    players[currentGamer].cash += 50;
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                }
                if (randChance == 4) {
                    drawChance(window, font, 0, randChance);
                    players[currentGamer].cash -= 15;
                    if (checkLose(players[currentGamer])) {
                        currentWindowState = GAME_OVER;
                    }
                    else {
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }
                }
                if (randChance == 5) {
                    for (int i = 0; i < players.size(); ++i) {
                        if (i != currentGamer) {
                            players[i].cash += 50;
                        }
                    }
                    players[currentGamer].cash -= 50 * playerQ;
                    drawChance(window, font, 0, randChance);
                    if (checkLose(players[currentGamer])) {
                        currentWindowState = GAME_OVER;
                    }
                    else {
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }
                }
                if (randChance == 6) {
                    players[currentGamer].freeJail = true;
                    drawChance(window, font, 0, randChance);
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                }
            }
            else if (players[currentGamer].currentPosition == 2 || players[currentGamer].currentPosition == 17 || players[currentGamer].currentPosition == 33) {
                communityChest;
                int randChance = rand() % 7;
                if (randChance == 0) {
                    drawChance(window, font, 1, randChance);
                    players[currentGamer].cash -= 100;
                    if (checkLose(players[currentGamer])) {
                        currentWindowState = GAME_OVER;
                    }
                    else {
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }
                }
                if (randChance == 1) {
                    drawChance(window, font, 1, randChance);
                    players[currentGamer].cash -= 50;
                    if (checkLose(players[currentGamer])) {
                        currentWindowState = GAME_OVER;
                    }
                    else {
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }
                }
                if (randChance == 2) {
                    drawChance(window, font, 1, randChance);
                    players[currentGamer].cash += 100;
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                }
                if (randChance == 3) {
                    drawChance(window, font, 1, randChance);
                    players[currentGamer].cash -= 50;
                    if (checkLose(players[currentGamer])) {
                        currentWindowState = GAME_OVER;
                    }
                    else {
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }
                }
                if (randChance == 4) {
                    drawChance(window, font, 1, randChance);
                    players[currentGamer].cash += 200;
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                }
                if (randChance == 5) {
                    drawChance(window, font, 1, randChance);
                    players[currentGamer].freeJail = true;
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                }
                if (randChance == 6) {
                    drawChance(window, font, 1, randChance);
                    players[currentGamer].cash += 100;
                    currentGamer = (currentGamer + 1) % playerQ;
                    currentWindowState = IN_GAME;
                    cubeResult = 0;
                }
            }
            else if (players[currentGamer].currentPosition == 4) {
                currentWindowState = TAX_200;
            }
            else if (players[currentGamer].currentPosition == 38) {
                currentWindowState = TAX_100;
            }
            else if (players[currentGamer].currentPosition == 0 || players[currentGamer].currentPosition == 10 || players[currentGamer].currentPosition == 20) {
                //currentGamer = (currentGamer + 1) % playerQ;
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
            while (!pressed) {
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
                        pressed = true;
                    }
                }
                window.display();
            }
            currentGamer = (currentGamer + 1) % playerQ;
            pressed = false;
            break;
        }
        case TO_JAIL_TABLE:
        {
            tales[players[currentGamer].currentPosition].setFillColor(Color(0, 0, 0, 1));
            window.draw(tales[players[currentGamer].currentPosition]);
            RectangleShape InfoWindow(Vector2f(500, 300));
            InfoWindow.setPosition(225, 325);
            InfoWindow.setFillColor(Color(0, 0, 0, 3));
            window.draw(InfoWindow);
            Text tax100("", font, 30);
            tax100.setFillColor(Color::White);
            tax100.setStyle(Text::Bold);
            tax100.setString("Велкам ту тюрьма.\nВы пропускаете ход и\nне полуете 200 за круг");
            tax100.setPosition(300, 400);
            window.draw(tax100);

            RectangleShape BUY(Vector2f(170, 50));
            BUY.setPosition(250, 550);
            BUY.setFillColor(Color::White);
            window.draw(BUY);

            Text buy("", font, 25);
            buy.setFillColor(Color::Black);
            buy.setStyle(Text::Bold);
            buy.setString("Освободиться");
            buy.setPosition(256, 557);
            window.draw(buy);

            RectangleShape CANCEL(Vector2f(100, 50));
            CANCEL.setPosition(600, 550);
            CANCEL.setFillColor(Color::White);
            window.draw(CANCEL);

            Text cancel("", font, 25);
            cancel.setFillColor(Color::Black);
            cancel.setStyle(Text::Bold);
            cancel.setString("ОК");
            cancel.setPosition(630, 557);
            window.draw(cancel);
            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && clicked(CANCEL, Mouse::getPosition(window), 100, 50)) {
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }
                    if (event.mouseButton.button == Mouse::Left && clicked(BUY, Mouse::getPosition(window), 170, 50)) {
                        if (players[currentGamer].freeJail == true) {
                            players[currentGamer].freeJail == false;
                            players[currentGamer].inJail == false;
                            currentGamer = (currentGamer + 1) % playerQ;
                            currentWindowState = IN_GAME;
                            cubeResult = 0;
                        }
                        else {
                            players[currentGamer].cash -= 50;
                            players[currentGamer].inJail = false;
                            if (checkLose(players[currentGamer])) {
                                currentWindowState = GAME_OVER;
                                break;
                            }
                            currentGamer = (currentGamer + 1) % playerQ;
                            currentWindowState = IN_GAME;
                            cubeResult = 0;
                        }
                    }

                }
            }
            window.display();
            break;
        }
        case GAME_OVER:
        {
            for (int i = 0; i < 40; ++i) {
                if (streets[i].owner == players[currentGamer].name) {
                    streets[i].owner = "Нет";
                }
            }
            RectangleShape InfoWindow(Vector2f(500, 300));
            InfoWindow.setPosition(225, 325);
            InfoWindow.setFillColor(Color(0, 0, 0, 3));
            window.draw(InfoWindow);
            Text tax100("", font, 30);
            tax100.setFillColor(Color::White);
            tax100.setStyle(Text::Bold);
            tax100.setString("Игрок " + players[currentGamer].name + "\nобанкротился");
            tax100.setPosition(375, 400);
            window.draw(tax100);

            RectangleShape BUY(Vector2f(120, 50));
            BUY.setPosition(420, 550);
            BUY.setFillColor(Color::White);
            window.draw(BUY);

            Text buy("", font, 25);
            buy.setFillColor(Color::Black);
            buy.setStyle(Text::Bold);
            buy.setString("ОК");
            buy.setPosition(455, 557);
            window.draw(buy);

            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && clicked(BUY, Mouse::getPosition(window), 120, 50)) {
                        players.erase(players.begin() + currentGamer);
                        --playerQ;
                        currentGamer = currentGamer % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }
                }
            }
            window.display();
            break;
        }
        case TAX_100:
        {
            tales[players[currentGamer].currentPosition].setFillColor(Color(0, 0, 0, 1));
            window.draw(tales[players[currentGamer].currentPosition]);
            RectangleShape InfoWindow(Vector2f(500, 300));
            InfoWindow.setPosition(225, 325);
            InfoWindow.setFillColor(Color(0, 0, 0, 3));
            window.draw(InfoWindow);
            Text tax100("", font, 30);
            tax100.setFillColor(Color::White);
            tax100.setStyle(Text::Bold);
            tax100.setString("Вам необходимо заплатить\nналог на роскошь\nв размере 100");
            tax100.setPosition(300, 400);
            window.draw(tax100);

            RectangleShape BUY(Vector2f(120, 50));
            BUY.setPosition(420, 550);
            BUY.setFillColor(Color::White);
            window.draw(BUY);

            Text buy("", font, 25);
            buy.setFillColor(Color::Black);
            buy.setStyle(Text::Bold);
            buy.setString("Оплатить");
            buy.setPosition(422, 557);
            window.draw(buy);

            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && clicked(BUY, Mouse::getPosition(window), 120, 50)) {
                        players[currentGamer].cash -= 100;
                        if (checkLose(players[currentGamer])) {
                            currentWindowState = GAME_OVER;
                            break;
                        }
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }

                }
            }
            window.display();
            break;
        }
        case TAX_200:
        {
            tales[players[currentGamer].currentPosition].setFillColor(Color(0, 0, 0, 1));
            window.draw(tales[players[currentGamer].currentPosition]);
            RectangleShape InfoWindow(Vector2f(500, 300));
            InfoWindow.setPosition(225, 325);
            InfoWindow.setFillColor(Color(0, 0, 0, 3));
            window.draw(InfoWindow);
            Text tax100("", font, 30);
            tax100.setFillColor(Color::White);
            tax100.setStyle(Text::Bold);
            tax100.setString("Вам необходимо заплатить\nподоходный налог\nв размере 200");
            tax100.setPosition(300, 400);
            window.draw(tax100);

            RectangleShape BUY(Vector2f(120, 50));
            BUY.setPosition(420, 550);
            BUY.setFillColor(Color::White);
            window.draw(BUY);

            Text buy("", font, 25);
            buy.setFillColor(Color::Black);
            buy.setStyle(Text::Bold);
            buy.setString("Оплатить");
            buy.setPosition(422, 557);
            window.draw(buy);

            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && clicked(BUY, Mouse::getPosition(window), 120, 50)) {
                        players[currentGamer].cash -= 200;
                        if (checkLose(players[currentGamer])) {
                            currentWindowState = GAME_OVER;
                            break;
                        }
                        currentGamer = (currentGamer + 1) % playerQ;
                        currentWindowState = IN_GAME;
                        cubeResult = 0;
                    }

                }
            }
            window.display();
            break;
        }
        case TALE_MARK:
        {
            /*
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
            */
            currentWindowState = TALE_PREWIEW;

            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;
                }
            }
            break;
        }
        case TALE_PREWIEW:
        {
            tales[players[currentGamer].currentPosition].setFillColor(Color(0, 0, 0, 1));
            window.draw(tales[players[currentGamer].currentPosition]);
            RectangleShape InfoWindow(Vector2f(500, 300));
            InfoWindow.setPosition(225, 325);
            InfoWindow.setFillColor(Color(0, 0, 0, 3));
            window.draw(InfoWindow);
            drawInfo(window, font, streets[players[currentGamer].currentPosition]);

            if (streets[players[currentGamer].currentPosition].owner == "Нет") {
                RectangleShape BUY(Vector2f(100, 50));
                BUY.setPosition(250, 550);
                BUY.setFillColor(Color::White);
                window.draw(BUY);

                Text buy("", font, 25);
                buy.setFillColor(Color::Black);
                buy.setStyle(Text::Bold);
                buy.setString("Купить");
                buy.setPosition(256, 557);
                window.draw(buy);

                RectangleShape CANCEL(Vector2f(100, 50));
                CANCEL.setPosition(600, 550);
                CANCEL.setFillColor(Color::White);
                window.draw(CANCEL);

                Text cancel("", font, 25);
                cancel.setFillColor(Color::Black);
                cancel.setStyle(Text::Bold);
                cancel.setString("Отмена");
                cancel.setPosition(606, 557);
                window.draw(cancel);
                Event event;
                while (window.pollEvent(event)) {
                    switch (event.type) {
                    case Event::Closed:
                        window.close();
                        break;

                    case Event::MouseButtonPressed:
                        if (event.mouseButton.button == Mouse::Left && clicked(CANCEL, Mouse::getPosition(window), 100, 50)) {
                            currentGamer = (currentGamer + 1) % playerQ;
                            currentWindowState = IN_GAME;
                            cubeResult = 0;
                        }
                        if (event.mouseButton.button == Mouse::Left && clicked(BUY, Mouse::getPosition(window), 100, 50)) {
                            players[currentGamer].cash -= streets[players[currentGamer].currentPosition].cost;
                            streets[players[currentGamer].currentPosition].owner = players[currentGamer].name;
                            if (checkLose(players[currentGamer])) {
                                currentWindowState = GAME_OVER;
                                break;
                            }
                            currentGamer = (currentGamer + 1) % playerQ;
                            currentWindowState = IN_GAME;
                            cubeResult = 0;
                        }
                    }
                }
            }
            else {
                if (players[currentGamer].name == streets[players[currentGamer].currentPosition].owner) {
                    RectangleShape OK(Vector2f(120, 50));
                    OK.setPosition(420, 550);
                    OK.setFillColor(Color::White);
                    window.draw(OK);

                    Text ok("", font, 25);
                    ok.setFillColor(Color::Black);
                    ok.setStyle(Text::Bold);
                    ok.setString("OK");
                    ok.setPosition(426, 557);
                    window.draw(ok);
                    Event event;
                    while (window.pollEvent(event)) {
                        switch (event.type) {
                        case Event::Closed:
                            window.close();
                            break;

                        case Event::MouseButtonPressed:
                            if (event.mouseButton.button == Mouse::Left && clicked(OK, Mouse::getPosition(window), 120, 50)) {
                                currentGamer = (currentGamer + 1) % playerQ;
                                currentWindowState = IN_GAME;
                                cubeResult = 0;
                            }
                        }
                    }
                }
                else {
                    RectangleShape PAY(Vector2f(120, 50));
                    PAY.setPosition(420, 550);
                    PAY.setFillColor(Color::White);
                    window.draw(PAY);

                    Text pay("", font, 25);
                    pay.setFillColor(Color::Black);
                    pay.setStyle(Text::Bold);
                    pay.setString("Оплатить");
                    pay.setPosition(426, 557);
                    window.draw(pay);

                    Event event;
                    while (window.pollEvent(event)) {
                        switch (event.type) {
                        case Event::Closed:
                            window.close();
                            break;

                        case Event::MouseButtonPressed:
                            if (event.mouseButton.button == Mouse::Left && clicked(PAY, Mouse::getPosition(window), 120, 50)) {
                                players[currentGamer].cash -= streets[players[currentGamer].currentPosition].rent;
                                if (checkLose(players[currentGamer])) {
                                    currentWindowState = GAME_OVER;
                                    break;
                                }
                                currentGamer = (currentGamer + 1) % playerQ;
                                currentWindowState = IN_GAME;
                                cubeResult = 0;
                            }
                        }
                    }
                }
            }
            window.display();
            break;
        }
        case EXIT:
        {
            window.clear();
            RectangleShape InfoWindow(Vector2f(500, 300));
            InfoWindow.setPosition(225, 325);
            InfoWindow.setFillColor(Color(255, 255, 255, 40));
            window.draw(InfoWindow);
            Text tax100("", font, 30);
            tax100.setFillColor(Color::White);
            tax100.setStyle(Text::Bold);
            tax100.setString("Игра окончена!");
            tax100.setPosition(385, 400);
            window.draw(tax100);

            RectangleShape BUY(Vector2f(120, 50));
            BUY.setPosition(420, 550);
            BUY.setFillColor(Color(255, 255, 255, 20));
            window.draw(BUY);

            Text buy("", font, 25);
            buy.setFillColor(Color::White);
            buy.setStyle(Text::Bold);
            buy.setString("Выйти");
            buy.setPosition(438, 557);
            window.draw(buy);

            Event event;
            while (window.pollEvent(event)) {
                switch (event.type) {
                case Event::Closed:
                    window.close();
                    break;

                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && clicked(BUY, Mouse::getPosition(window), 120, 50)) {
                        window.close();
                    }

                }
            }
            window.display();
            break;
        }
        }
    }
    return 0;
}