#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
using namespace std;

class Enemy{
	private:
		float hp;
		int dmg;
		int pos_x;
		int pos_y;
		bool isAlive = true;
	public:
		Enemy(int hp1, int dmg1, int pos_y1, int pos_x1) {
			hp = hp1;
			dmg = dmg1;
			pos_x = pos_x1;
			pos_y = pos_y1;
		}
		
		Enemy() {}
		
		void got_dmg(int dmg) {
			hp -= dmg;
		}
		
		void set_isAlive(bool isAlive1) {
			isAlive = isAlive1;
		}
		
		bool get_isAlive() {
			return isAlive;
		}
		
		int get_hp() {
			return hp;
		}
		
		int get_dmg() {
			return dmg;
		}
		
		int get_x() {
			return pos_x;
		}
		
		int get_y() {
			return pos_y;
		}
};
bool debug_opened = false;
string stage = "map";
string action_additional_text;
string action_stage = "main";
int pos_x = 1;
int pos_y = 1;
float hp = 20.0;
float arm = 0.9;
int dmg = 3;
int mp = 3;
std::random_device rd;
std::mt19937 gen(rd());
string inventory[8] = {"nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing", "nothing"};
string standart_map[12][12] = {
{" # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # "},
{" # ", " . ", " . ", " # ", " . ", " . ", " # ", " . ", " . ", " . ", " . ", " # "},
{" # ", " . ", " . ", " # ", " . ", " . ", " . ", " . ", " . ", " . ", " . ", " # "},
{" # ", " . ", " . ", " # ", " . ", " . ", " # ", " . ", " . ", " . ", " . ", " # "},
{" # ", " . ", " # ", " # ", " . ", " . ", " # ", " + ", " . ", " . ", " . ", " # "},
{" # ", " . ", " . ", " . ", " . ", " . ", " # ", " # ", " # ", " # ", " . ", " # "},
{" # ", " . ", " . ", " . ", " . ", " . ", " # ", " + ", " . ", " . ", " . ", " # "},
{" # ", " . ", " # ", " # ", " . ", " . ", " # ", " + ", " . ", " . ", " . ", " # "},
{" # ", " . ", " + ", " # ", " . ", " . ", " # ", " + ", " . ", " . ", " . ", " # "},
{" # ", " . ", " . ", " # ", " . ", " . ", " # ", " . ", " . ", " . ", " . ", " # "},
{" # ", " . ", " . ", " # ", " + ", " . ", " # ", " . ", " . ", " . ", " . ", " # "},
{" # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # ", " # "}
};
Enemy Guard1(5, 2, 5, 1);
Enemy Guard2(10, 4, 7, 1);
Enemy Guard3(20, 6, 2, 6);
Enemy Guard4(25, 6, 5, 10);
Enemy Guard5(35, 9, 10, 10);
Enemy enemies_on_standart_map[5] = {Guard1, Guard2, Guard3, Guard4, Guard5};
Enemy* current_enemy;

void chest_loot() {
	cout << "Chest loot: ";
	std::uniform_int_distribution<> distrib(0, 2);
	int chest_random = distrib(gen);
	switch (chest_random) {
		case 0:
			cout << "Heal Potion" << endl;
			for (int i = 0; i < size(inventory); i++) {
				if (inventory[i] == "nothing") {
					inventory[i] = "Heal Potion";
					break;
				}
			}
			break;
		case 1:
			cout << "Diary of Survivor (HP, DMG, ARM upgraded)" << endl;
			hp += 5;
			arm -= 0.1;
			dmg += 2;
			break;
		case 2:
			cout << "MP Potion (MP +3)" << endl;
			mp += 3;
			break;
	}
}

void move(char m) 
{
    switch (m) {
		case 'h':
			if (not debug_opened) {
				debug_opened = true;
			}
			else {
				debug_opened = false;
			}
			break;
        case 'w':
            if (standart_map[pos_y - 1][pos_x] != " # " and standart_map[pos_y - 1][pos_x] != " I " and standart_map[pos_y - 1][pos_x] != " + ") {
                standart_map[pos_y][pos_x] = " . ";
                pos_y -= 1;
            }
			else if (standart_map[pos_y - 1][pos_x] == " I ") {
				cout << "There is a enemy on this field. Do you want to fight him?" << endl << "y - yes, anything else - no" << endl;
				char answer;
				cin >> answer;
				if (answer == 'y') {
					for (int i = 0; i < size(enemies_on_standart_map); i++) {
						if (enemies_on_standart_map[i].get_y() == pos_y - 1 and enemies_on_standart_map[i].get_x() == pos_x) {
							current_enemy = &enemies_on_standart_map[i];
							break;
						}
					}
					stage = "fight";
					action_additional_text = "You decided to fight an enemy!";
				}
			}
			else if (standart_map[pos_y - 1][pos_x] == " + ") {
				chest_loot();
				standart_map[pos_y - 1][pos_x] = " . ";
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
            break;
        case 'a':
            if (standart_map[pos_y][pos_x - 1] != " # " and standart_map[pos_y][pos_x - 1] != " I " and standart_map[pos_y][pos_x - 1] != " + ") {
                standart_map[pos_y][pos_x] = " . ";
                pos_x -= 1;
            }
			else if (standart_map[pos_y][pos_x - 1] == " I ") {
				cout << "There is a enemy on this field. Do you want to fight him?" << endl << "y - yes, anything else - no" << endl;
				char answer;
				cin >> answer;
				if (answer == 'y') {
					for (int i = 0; i < size(enemies_on_standart_map); i++) {
						if (enemies_on_standart_map[i].get_y() == pos_y and enemies_on_standart_map[i].get_x() == pos_x - 1) {
							current_enemy = &enemies_on_standart_map[i];
							break;
						}
					}
					stage = "fight";
					action_additional_text = "You decided to fight an enemy!";
				}
			}
			else if (standart_map[pos_y][pos_x - 1] == " + ") {
				chest_loot();
				standart_map[pos_y][pos_x - 1] = " . ";
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
            break;
        case 's':
            if (standart_map[pos_y + 1][pos_x] != " # " and standart_map[pos_y + 1][pos_x] != " I " and standart_map[pos_y + 1][pos_x] != " + ") {
                standart_map[pos_y][pos_x] = " . ";
                pos_y += 1;
            }
			else if (standart_map[pos_y + 1][pos_x] == " I ") {
				cout << "There is a enemy on this field. Do you want to fight him?" << endl << "y - yes, anything else - no" << endl;
				char answer;
				cin >> answer;
				if (answer == 'y') {
					for (int i = 0; i < size(enemies_on_standart_map); i++) {
						if (enemies_on_standart_map[i].get_y() == pos_y + 1 and enemies_on_standart_map[i].get_x() == pos_x) {
							current_enemy = &enemies_on_standart_map[i];
							break;
						}
					}
					stage = "fight";
					action_additional_text = "You decided to fight an enemy!";
				}
			}
			else if (standart_map[pos_y + 1][pos_x] == " + ") {
				chest_loot();
				standart_map[pos_y + 1][pos_x] = " . ";
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
            break;
        case 'd':
            if (standart_map[pos_y][pos_x + 1] != " # " and standart_map[pos_y][pos_x + 1] != " I " and standart_map[pos_y][pos_x + 1] != " + ") {
                standart_map[pos_y][pos_x] = " . ";
                pos_x += 1;
            }
			else if (standart_map[pos_y][pos_x + 1] == " I ") {
				cout << "There is a enemy on this field. Do you want to fight him?" << endl << "y - yes, anything else - no" << endl;
				char answer;
				cin >> answer;
				if (answer == 'y') {
					for (int i = 0; i < size(enemies_on_standart_map); i++) {
						if (enemies_on_standart_map[i].get_y() == pos_y and enemies_on_standart_map[i].get_x() == pos_x + 1) {
							current_enemy = &enemies_on_standart_map[i];
							break;
						}
					}
					stage = "fight";
					action_additional_text = "You decided to fight an enemy!";
				}
			}
			else if (standart_map[pos_y][pos_x + 1] == " + ") {
				chest_loot();
				standart_map[pos_y][pos_x + 1] = " . ";
				std::this_thread::sleep_for(std::chrono::seconds(2));
			}
            break;
    }
}

void render_action() {
	cout << " #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # " << endl;
	cout << " #                                                  # " << endl;
	cout << " #                       I                          # " << endl;
	cout << " #                                                  # " << endl;
	cout << " #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  #  # " << endl;
	cout << " " << action_additional_text << endl;
	if (action_stage == "main") {
		cout << " Your HP: " << hp << "         " << "Your MP: " << mp << endl;
		cout << " Enemy HP: " << current_enemy->get_hp() << endl << endl;
		cout << " a - attack, b - magic, c - inventory" << endl;
	}
	else {
		cout << " 1. " << inventory[0] << "     " << "2. " << inventory[1] << endl;
		cout << " 3. " << inventory[2] << "     " << "4. " << inventory[3] << endl;
		cout << " 5. " << inventory[4] << "     " << "6. " << inventory[5] << endl;
		cout << " 7. " << inventory[6] << "     " << "8. " << inventory[7] << endl << endl;
	}
}

void action(char move) {
	switch (move) {
		case 'a':
			if (action_stage == "main") {
				system("cls");
				action_additional_text = "You attacked enemy on " + to_string(dmg) + " HP!";
				current_enemy->got_dmg(dmg);
				render_action();
				std::this_thread::sleep_for(std::chrono::seconds(2));
				if (current_enemy->get_hp() <= 0) {
					system("cls");
					action_additional_text = "You won!";
					current_enemy->set_isAlive(false);
					render_action();
					std::this_thread::sleep_for(std::chrono::seconds(2));
					int count_dead_enemies = 0;
					for (int i = 0; i < size(enemies_on_standart_map); i++) {
							if (not enemies_on_standart_map[i].get_isAlive()){
								count_dead_enemies += 1;
							}
						}
						if (count_dead_enemies == size(enemies_on_standart_map)) {
							stage = "gg";
							return;
						}
					stage = "map";
					return;
				}
				action_additional_text = "Enemy attacked you on " + to_string(current_enemy->get_dmg() * arm) + " HP! Your turn!";
				hp -= current_enemy->get_dmg() * arm;
				if (hp <= 0) {
					action_additional_text = "Enemy attacked you on " + to_string(current_enemy->get_dmg() * arm) + " HP!";
					system("cls");
					render_action();
					std::this_thread::sleep_for(std::chrono::seconds(2));
					stage = "game_over";
					return;
				}
			}
			break;
		case 'b':
			if (action_stage == "main") {
				if (mp > 0) {
					mp -= 1;
					system("cls");
					action_additional_text = "You used Fireball and damaged on " + to_string(dmg * 2) + " HP!";
					current_enemy->got_dmg(dmg * 2);
					render_action();
					std::this_thread::sleep_for(std::chrono::seconds(2));
					if (current_enemy->get_hp() <= 0) {
						system("cls");
						action_additional_text = "You won!";
						current_enemy->set_isAlive(false);
						render_action();
						std::this_thread::sleep_for(std::chrono::seconds(2));
						int count_dead_enemies = 0;
						for (int i = 0; i < size(enemies_on_standart_map); i++) {
							if (not enemies_on_standart_map[i].get_isAlive()){
								count_dead_enemies += 1;
							}
						}
						if (count_dead_enemies == size(enemies_on_standart_map)) {
							stage = "gg";
							return;
						}
						stage = "map";
						return;
						
					}
					action_additional_text = "Enemy attacked you on " + to_string(current_enemy->get_dmg() * arm) + " HP! Your turn!";
					hp -= current_enemy->get_dmg() * arm;
					if (hp <= 0) {
						action_additional_text = "Enemy attacked you on " + to_string(current_enemy->get_dmg() * arm) + " HP!";
						system("cls");
						render_action();
						std::this_thread::sleep_for(std::chrono::seconds(2));
						stage = "game_over";
						return;
					}
				}
				else {
					action_additional_text = "You don't have enough MP!";
				}
			}
			break;
		case 'c':
			action_stage = "inventory";
			action_additional_text = "Choose the number of item which you would like to use (0 - back): ";
			break;
		case '0':
			if (action_stage == "inventory") {
				action_stage = "main";
				action_additional_text = "You decided to fight an enemy!";
			}
			break;
		case '1':
			if (inventory[0] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[0] = "nothing";
				hp += 13;
			}
			break;
		case '2':
			if (inventory[1] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[1] = "nothing";
				hp += 13;
			}
			break;
		case '3':
			if (inventory[2] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[2] = "nothing";
				hp += 13;
			}
			break;
		case '4':
			if (inventory[3] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[3] = "nothing";
				hp += 13;
			}
			break;
		case '5':
			if (inventory[4] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[4] = "nothing";
				hp += 13;
			}
			break;
		case '6':
			if (inventory[5] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[5] = "nothing";
				hp += 13;
			}
			break;
		case '7':
			if (inventory[6] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[6] = "nothing";
				hp += 13;
			}
			break;
		case '8':
			if (inventory[7] == "Heal Potion") {
				action_additional_text = "You drank Heal Potion! +13 HP";
				action_stage = "main";
				inventory[7] = "nothing";
				hp += 13;
			}
			break;
	}
}

void render_ui()
{
	if (debug_opened) {
		cout << "Debug: " << endl;
		cout << "pos_x: " << pos_x << " pos_y: " << pos_y << endl;
		cout << "stage: " << stage << " action_stage: " << action_stage << endl;
		int enemies_left = 5;
		for (int i = 0; i < size(enemies_on_standart_map); i++) {
			if (not enemies_on_standart_map[i].get_isAlive()){
				enemies_left -= 1;
			}
		}
		cout << "action_additional_text: " << action_additional_text << " enemies left: " << enemies_left << endl;
	}
	for (int i = 0; i < size(standart_map[i]); i++)
	{
		for (int j = 0; j < size(standart_map); j++)
		{
			cout << standart_map[i][j];
		}
		switch (i) {
			case 0:
				cout << " HP: " << hp;
				break;
			case 1:
				cout << " MP: " << mp;
				break;
			case 2:
				cout << " ARM: " << arm;
				break;
			case 3:
				cout << " DMG: " << dmg;
				break;
		}
		cout << endl;
	}
    cout << "inventory: ";
    for (int i = 0; i < size(inventory); i++) {
        cout << inventory[i] << " ";
    }
    cout << endl << "w - up, s - down, a - left, d - right" << endl;
}

int main()
{	
	while (true) {
		system("cls");
		if (stage == "map") {
			for (int i = 0; i < size(enemies_on_standart_map); i++) {
				if (enemies_on_standart_map[i].get_isAlive()) {
					standart_map[enemies_on_standart_map[i].get_y()][enemies_on_standart_map[i].get_x()] = " I ";
				}
				else {
					standart_map[enemies_on_standart_map[i].get_y()][enemies_on_standart_map[i].get_x()] = " . ";
				}
			}
			standart_map[pos_y][pos_x] = " @ ";
			render_ui();
			char m;
			cin >> m;
			move(m);
		}
		else if (stage == "fight") {
			render_action();
			char act;
			cin >> act;
			action(act);
		}
		else if (stage == "game_over") {
			cout << "Game Over" << endl << "0 - exit" << endl;
			char move;
			cin >> move;
			if (move == '0') {
				system("cls");
				return 0;
			}
		}
		else if (stage == "gg") {
			cout << "Congratulations! You won! All enemies killed." << endl << "0 - exit" << endl;
			char move;
			cin >> move;
			if (move == '0') {
				system("cls");
				return 0;
			}
		}
	}
}