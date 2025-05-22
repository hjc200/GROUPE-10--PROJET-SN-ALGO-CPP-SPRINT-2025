#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <regex>
#include <stack>
#include <vector>
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#endif

// Couleurs ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

const std::string DATA_FILE = "reservations.txt";
enum Language { FRENCH, ENGLISH };
Language currentLang = FRENCH;

struct Event {
    std::string name;
    std::string date;
    std::string location;
    std::vector<std::string> availableSeats;

    Event(std::string n, std::string d, std::string l, std::vector<std::string> seats)
        : name(n), date(d), location(l), availableSeats(seats) {}
};

struct ReservationNode {
    int reservationNumber;
    std::string eventName;
    std::string clientName;
    std::string date;
    std::string seatNumber;
    ReservationNode* next;

    static int nextReservationNumber;

    ReservationNode(std::string event, std::string name, std::string d, std::string seat)
        : eventName(event), clientName(name), date(d), seatNumber(seat), next(nullptr) {
        reservationNumber = nextReservationNumber++;
    }

    std::string toFileString() const {
        return std::to_string(reservationNumber) + "|" + eventName + "|" + clientName + "|" + date + "|" + seatNumber;
    }
};

int ReservationNode::nextReservationNumber = 1;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printCentered(const std::string& text, int width = 80, const std::string& color = RESET) {
    int padding = (width - static_cast<int>(text.length())) / 2;
    if (padding < 0) padding = 0;
    std::cout << color << std::string(padding, ' ') << text << RESET << std::endl;
}

void loadingAnimation(int points = 5, int delay = 200) {
    printCentered("Chargement", 80, BLUE);
    for (int i = 0; i < points; i++) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

class ReservationSystem {
private:
    ReservationNode* head;
    std::vector<Event> eventList;

public:
    ReservationSystem() : head(nullptr) {
        initializeEvents();
        loadFromFile();
    }

    ~ReservationSystem() {
        saveToFile();
        while (head) {
            ReservationNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void initializeEvents() {
        eventList.push_back(Event("Concert Symphonique", "15/12/2023", "Opéra National", {"A1", "A2", "B1", "B2"}));
        eventList.push_back(Event("Festival Rock", "20/11/2023", "Stade de la Ville", {"C1", "C2", "D1", "D2"}));
        eventList.push_back(Event("Match de Football", "25/11/2023", "Stade Municipal", {"E1", "E2", "F1", "F2"}));
        eventList.push_back(Event("Dîner Gastronomique", "30/11/2023", "Restaurant Étoilé", {"Table 1", "Table 2", "Table 3"}));
    }

    void displayEvents() {
        printCentered(currentLang == FRENCH ? "=== ÉVÉNEMENTS DISPONIBLES ===" : "=== AVAILABLE EVENTS ===", 80, MAGENTA);

        for (size_t i = 0; i < eventList.size(); ++i) {
            const Event& e = eventList[i];
            printCentered(std::to_string(i + 1) + ". " + e.name, 80, CYAN);
            printCentered("Date: " + e.date, 80, YELLOW);
            printCentered("Lieu/Location: " + e.location, 80, BLUE);
            std::string seats;
            for (const auto& s : e.availableSeats) seats += s + " ";
            printCentered("Places: " + seats, 80, GREEN);
            std::cout << std::endl;
        }
    }

    void makeReservation() {
        displayEvents();
        int choice;
        std::cout << (currentLang == FRENCH ? "Choisissez un événement (numéro): " : "Choose an event (number): ");
        std::cin >> choice;
        std::cin.ignore();

        if (choice < 1 || choice > static_cast<int>(eventList.size())) {
            throw std::invalid_argument(currentLang == FRENCH ? "Choix invalide!" : "Invalid choice!");
        }

        Event& selected = eventList[choice - 1];
        std::string name;
        std::cout << (currentLang == FRENCH ? "Nom du client : " : "Client name: ");
        std::getline(std::cin, name);

        if (selected.availableSeats.empty()) {
            printCentered(currentLang == FRENCH ? "Aucune place disponible." : "No seats available.", 80, RED);
            return;
        }

        printCentered(currentLang == FRENCH ? "Places disponibles:" : "Available seats:", 80, GREEN);
        for (size_t i = 0; i < selected.availableSeats.size(); ++i) {
            std::cout << "[" << i + 1 << "] " << selected.availableSeats[i] << "  ";
        }
        std::cout << std::endl;

        int seatChoice;
        std::cout << (currentLang == FRENCH ? "Choisissez une place (numéro) : " : "Choose a seat (number): ");
        std::cin >> seatChoice;
        std::cin.ignore();

        if (seatChoice < 1 || seatChoice > static_cast<int>(selected.availableSeats.size())) {
            throw std::invalid_argument(currentLang == FRENCH ? "Place invalide!" : "Invalid seat!");
        }

        std::string seat = selected.availableSeats[seatChoice - 1];
        ReservationNode* newNode = new ReservationNode(selected.name, name, selected.date, seat);
        newNode->next = head;
        head = newNode;
        selected.availableSeats.erase(selected.availableSeats.begin() + seatChoice - 1);

        printCentered(currentLang == FRENCH ? "Réservation confirmée!" : "Reservation confirmed!", 80, GREEN);
    }

    void saveToFile() {
        std::ofstream out(DATA_FILE);
        if (!out) {
            std::cerr << "Erreur de sauvegarde." << std::endl;
            return;
        }

        ReservationNode* curr = head;
        while (curr) {
            out << curr->toFileString() << std::endl;
            curr = curr->next;
        }
    }

    void loadFromFile() {
        std::ifstream in(DATA_FILE);
        if (!in) return;

        std::string line;
        while (std::getline(in, line)) {
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> parts;
            while (std::getline(ss, token, '|')) {
                parts.push_back(token);
            }
            if (parts.size() == 5) {
                ReservationNode* node = new ReservationNode(parts[1], parts[2], parts[3], parts[4]);
                node->reservationNumber = std::stoi(parts[0]);
                node->next = head;
                head = node;
                if (node->reservationNumber >= ReservationNode::nextReservationNumber) {
                    ReservationNode::nextReservationNumber = node->reservationNumber + 1;
                }
            }
        }
    }
};

void displayWelcome() {
    clearScreen();
    printCentered("=== SYSTEME DE RESERVATION ===", 80, CYAN);
    loadingAnimation();
}

void languageMenu() {
    clearScreen();
    printCentered("1. Français", 80, YELLOW);
    printCentered("2. English", 80, YELLOW);
    std::cout << "Choisissez votre langue / Choose your language: ";
    int lang;
    std::cin >> lang;
    currentLang = (lang == 2) ? ENGLISH : FRENCH;
}

void mainMenu() {
    printCentered(currentLang == FRENCH ? "=== MENU PRINCIPAL ===" : "=== MAIN MENU ===", 80, MAGENTA);
    printCentered(currentLang == FRENCH ? "[1] Voir les événements" : "[1] View events", 80, BLUE);
    printCentered(currentLang == FRENCH ? "[2] Langue" : "[2] Language", 80, BLUE);
    printCentered(currentLang == FRENCH ? "[3] Quitter" : "[3] Exit", 80, RED);
}

int main() {
    displayWelcome();
    languageMenu();

    ReservationSystem system;
    int choice;

    do {
        mainMenu();
        std::cout << (currentLang == FRENCH ? "Votre choix : " : "Your choice: ");
        std::cin >> choice;

        try {
            switch (choice) {
                case 1:
                    system.makeReservation();
                    break;
                case 2:
                    languageMenu();
                    break;
                case 3:
                    printCentered(currentLang == FRENCH ? "Merci et à bientôt !" : "Thank you and goodbye!", 80, CYAN);
                    break;
                default:
                    printCentered(currentLang == FRENCH ? "Choix invalide." : "Invalid choice.", 80, RED);
            }
        } catch (const std::exception& e) {
            printCentered(e.what(), 80, RED);
        }

    } while (choice != 3);

    return 0;
}
