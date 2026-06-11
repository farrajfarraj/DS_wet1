// 
// 234218 Data Structures 1.
// Semester: 2026B (Spring).
// Wet Exercise #1.
// 

#ifndef SegmentationResort26SPRING_H_
#define SegmentationResort26SPRING_H_

#include "wet1util.h"
#include "AVLTree.h"
#include "Guest.h"

class Table {
private:
    int id;
    int capacity;
    int occupied;
    AVLTree<Guest, int>* guestsById;

public:
    Table(int tableId, int tableCapacity)
        : id(tableId), capacity(tableCapacity), occupied(0),
          guestsById(new AVLTree<Guest, int>()) {}

    ~Table() { delete guestsById; }

    int getId() const { return id; }
    int getCapacity() const { return capacity; }
    int getOccupied() const { return occupied; }
    bool isFull() const { return occupied >= capacity; }
    AVLTree<Guest, int>* getGuestsTree() { return guestsById; }

    void addGuest(Guest* guest);
    void removeGuest(int guestId);
    void absorb(Table* other, AVLTree<Guest, int>* mergedTree);
};

class SegmentationResort {
private:
    AVLTree<Guest, int> guestsById;
    AVLTree<Guest, int> guestsByRoom;
    AVLTree<Table, int> tablesById;

    Guest* roomHead;
    Guest* lastCleaned;
    int currentMeal;

    void insertRoomNode(Guest* guest);
    void removeRoomNode(Guest* guest);

    static void setGuestsTable(AVLNode<Guest, int>* node, Table* table);
    static AVLTree<Guest, int>* mergeGuestTrees(AVLTree<Guest, int>* tree1,
                                                AVLTree<Guest, int>* tree2);
    static AVLNode<Guest, int>* buildBalancedFromGuests(Guest** arr,
                                                        int left,
                                                        int right);

public:
    // <DO-NOT-MODIFY> {
    SegmentationResort();

    virtual ~SegmentationResort();

    StatusType checkIn(int geustId, int roomNum);

    StatusType checkOut(int geustId);

    StatusType addTable(int tableId, int capacity);

    StatusType removeTable(int tableId);

    StatusType enterDiningRoom(int guestId, int tableId);

    StatusType leaveDiningRoom(int guestId, int tableId);

    StatusType reheatFood();

    StatusType joinTables(int tableId1, int tableId2);
                    
    output_t<int> joinFriend(int guestId1, int guestId2);

    output_t<int> cleanNextRoom();

    // } </DO-NOT-MODIFY>
};

#endif // SegmentationResort26SPRING_H_
