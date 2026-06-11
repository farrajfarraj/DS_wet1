#ifndef DS_WET1_SPRING_2026_GUEST_H
#define DS_WET1_SPRING_2026_GUEST_H

class Table;

class Guest {
private:
    int id;
    int room;
    Table* currentTable;
    int lastEnteredMeal;

public:
    Guest* prevRoom;
    Guest* nextRoom;

    Guest(int guestId, int roomNum)
        : id(guestId), room(roomNum), currentTable(nullptr),
          lastEnteredMeal(0), prevRoom(nullptr), nextRoom(nullptr) {}

    int getId() const { return id; }
    int getRoom() const { return room; }
    Table* getTable() const { return currentTable; }
    void setTable(Table* table) { currentTable = table; }
    int getLastEnteredMeal() const { return lastEnteredMeal; }
    void setLastEnteredMeal(int meal) { lastEnteredMeal = meal; }
};

#endif // DS_WET1_SPRING_2026_GUEST_H
