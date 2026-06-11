#include "SegmentationResort26b1.h"

static void flattenGuests(AVLNode<Guest, int>* node, Guest** arr, int& index)
{
    if (node == nullptr) {
        return;
    }
    flattenGuests(node->leftNode, arr, index);
    arr[index++] = node->getVal();
    flattenGuests(node->rightNode, arr, index);
}

void Table::addGuest(Guest* guest)
{
    guestsById->insert(new AVLNode<Guest, int>(guest->getId(), guest));
    occupied++;
}

void Table::removeGuest(int guestId)
{
    guestsById->erase(guestId);
    occupied--;
}

void Table::absorb(Table* other, AVLTree<Guest, int>* mergedTree)
{
    capacity += other->capacity;
    occupied += other->occupied;
    delete guestsById;
    guestsById = mergedTree;
}

SegmentationResort::SegmentationResort()
    : roomHead(nullptr), lastCleaned(nullptr), currentMeal(1)
{
}

SegmentationResort::~SegmentationResort()
{
    tablesById.clearValues();
    guestsById.clearValues();
}

void SegmentationResort::insertRoomNode(Guest* guest)
{
    if (roomHead == nullptr) {
        roomHead = guest;
        guest->nextRoom = guest;
        guest->prevRoom = guest;
        return;
    }

    AVLNode<Guest, int>* succNode = guestsByRoom.lowerBound(guest->getRoom());
    Guest* succ = succNode ? succNode->getVal() : roomHead;
    Guest* pred = succ->prevRoom;

    guest->nextRoom = succ;
    guest->prevRoom = pred;
    pred->nextRoom = guest;
    succ->prevRoom = guest;

    if (guest->getRoom() < roomHead->getRoom()) {
        roomHead = guest;
    }
}

void SegmentationResort::removeRoomNode(Guest* guest)
{
    if (guest->nextRoom == guest) {
        roomHead = nullptr;
        lastCleaned = nullptr;
        guest->nextRoom = nullptr;
        guest->prevRoom = nullptr;
        return;
    }

    if (lastCleaned == guest) {
        lastCleaned = guest->prevRoom;
    }
    if (roomHead == guest) {
        roomHead = guest->nextRoom;
    }

    Guest* pred = guest->prevRoom;
    Guest* succ = guest->nextRoom;
    pred->nextRoom = succ;
    succ->prevRoom = pred;
    guest->nextRoom = nullptr;
    guest->prevRoom = nullptr;
}

void SegmentationResort::setGuestsTable(AVLNode<Guest, int>* node, Table* table)
{
    if (node == nullptr) {
        return;
    }
    setGuestsTable(node->leftNode, table);
    node->getVal()->setTable(table);
    setGuestsTable(node->rightNode, table);
}

AVLNode<Guest, int>* SegmentationResort::buildBalancedFromGuests(Guest** arr, int left, int right)
{
    if (left > right) {
        return nullptr;
    }

    int mid = left + (right - left) / 2;
    AVLNode<Guest, int>* node = new AVLNode<Guest, int>(arr[mid]->getId(), arr[mid]);
    node->leftNode = buildBalancedFromGuests(arr, left, mid - 1);
    node->rightNode = buildBalancedFromGuests(arr, mid + 1, right);
    node->fixValues();
    return node;
}

AVLTree<Guest, int>* SegmentationResort::mergeGuestTrees(AVLTree<Guest, int>* tree1,
                                                          AVLTree<Guest, int>* tree2)
{
    int size1 = tree1->getSize();
    int size2 = tree2->getSize();
    Guest** arr1 = nullptr;
    Guest** arr2 = nullptr;
    Guest** merged = nullptr;

    try {
        if (size1 > 0) {
            arr1 = new Guest*[size1];
        }
        if (size2 > 0) {
            arr2 = new Guest*[size2];
        }
        if (size1 + size2 > 0) {
            merged = new Guest*[size1 + size2];
        }

        int index1 = 0;
        int index2 = 0;
        flattenGuests(tree1->getRoot(), arr1, index1);
        flattenGuests(tree2->getRoot(), arr2, index2);

        int i = 0;
        int j = 0;
        int k = 0;
        while (i < size1 && j < size2) {
            if (arr1[i]->getId() < arr2[j]->getId()) {
                merged[k++] = arr1[i++];
            } else {
                merged[k++] = arr2[j++];
            }
        }
        while (i < size1) {
            merged[k++] = arr1[i++];
        }
        while (j < size2) {
            merged[k++] = arr2[j++];
        }

        AVLNode<Guest, int>* root = buildBalancedFromGuests(merged, 0, size1 + size2 - 1);
        AVLTree<Guest, int>* result = new AVLTree<Guest, int>(root);

        delete[] arr1;
        delete[] arr2;
        delete[] merged;
        return result;
    } catch (...) {
        delete[] arr1;
        delete[] arr2;
        delete[] merged;
        throw;
    }
}

StatusType SegmentationResort::checkIn(int geustId, int roomNum)
{
    if (geustId <= 0 || roomNum <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (guestsById.find(geustId) != nullptr || guestsByRoom.find(roomNum) != nullptr) {
        return StatusType::FAILURE;
    }

    Guest* guest = nullptr;
    AVLNode<Guest, int>* idNode = nullptr;
    AVLNode<Guest, int>* roomNode = nullptr;

    try {
        guest = new Guest(geustId, roomNum);
        idNode = new AVLNode<Guest, int>(geustId, guest);
        roomNode = new AVLNode<Guest, int>(roomNum, guest);

        insertRoomNode(guest);
        guestsById.insert(idNode);
        guestsByRoom.insert(roomNode);
    } catch (...) {
        delete idNode;
        delete roomNode;
        delete guest;
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType SegmentationResort::checkOut(int geustId)
{
    if (geustId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<Guest, int>* node = guestsById.find(geustId);
    if (node == nullptr) {
        return StatusType::FAILURE;
    }

    Guest* guest = node->getVal();
    if (guest->getTable() != nullptr) {
        return StatusType::FAILURE;
    }

    try {
        removeRoomNode(guest);
        guestsById.erase(geustId);
        guestsByRoom.erase(guest->getRoom());
        delete guest;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType SegmentationResort::addTable(int tableId, int capacity)
{
    if (tableId <= 0 || capacity <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (tablesById.find(tableId) != nullptr) {
        return StatusType::FAILURE;
    }

    Table* table = nullptr;
    AVLNode<Table, int>* node = nullptr;
    try {
        table = new Table(tableId, capacity);
        node = new AVLNode<Table, int>(tableId, table);
        tablesById.insert(node);
    } catch (...) {
        delete node;
        delete table;
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType SegmentationResort::removeTable(int tableId)
{
    if (tableId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<Table, int>* node = tablesById.find(tableId);
    if (node == nullptr) {
        return StatusType::FAILURE;
    }

    Table* table = node->getVal();
    if (table->getOccupied() != 0) {
        return StatusType::FAILURE;
    }

    try {
        tablesById.erase(tableId);
        delete table;
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType SegmentationResort::enterDiningRoom(int guestId, int tableId)
{
    if (guestId <= 0 || tableId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<Guest, int>* guestNode = guestsById.find(guestId);
    AVLNode<Table, int>* tableNode = tablesById.find(tableId);
    if (guestNode == nullptr || tableNode == nullptr) {
        return StatusType::FAILURE;
    }

    Guest* guest = guestNode->getVal();
    Table* table = tableNode->getVal();
    if (guest->getTable() != nullptr || table->isFull() ||
        guest->getLastEnteredMeal() == currentMeal) {
        return StatusType::FAILURE;
    }

    try {
        table->addGuest(guest);
        guest->setTable(table);
        guest->setLastEnteredMeal(currentMeal);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType SegmentationResort::leaveDiningRoom(int guestId, int tableId)
{
    if (guestId <= 0 || tableId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<Guest, int>* guestNode = guestsById.find(guestId);
    AVLNode<Table, int>* tableNode = tablesById.find(tableId);
    if (guestNode == nullptr || tableNode == nullptr) {
        return StatusType::FAILURE;
    }

    Guest* guest = guestNode->getVal();
    Table* table = tableNode->getVal();
    if (guest->getTable() != table) {
        return StatusType::FAILURE;
    }

    try {
        table->removeGuest(guestId);
        guest->setTable(nullptr);
        guest->setLastEnteredMeal(currentMeal);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType SegmentationResort::reheatFood()
{
    currentMeal++;
    return StatusType::SUCCESS;
}

StatusType SegmentationResort::joinTables(int tableId1, int tableId2)
{
    if (tableId1 <= 0 || tableId2 <= 0 || tableId1 == tableId2) {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<Table, int>* node1 = tablesById.find(tableId1);
    AVLNode<Table, int>* node2 = tablesById.find(tableId2);
    if (node1 == nullptr || node2 == nullptr) {
        return StatusType::FAILURE;
    }

    Table* table1 = node1->getVal();
    Table* table2 = node2->getVal();
    AVLTree<Guest, int>* mergedTree = nullptr;

    try {
        mergedTree = mergeGuestTrees(table1->getGuestsTree(), table2->getGuestsTree());
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }

    try {
        setGuestsTable(table2->getGuestsTree()->getRoot(), table1);
        table1->absorb(table2, mergedTree);
        tablesById.erase(tableId2);
        delete table2;
    } catch (...) {
        delete mergedTree;
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

output_t<int> SegmentationResort::joinFriend(int guestId1, int guestId2)
{
    if (guestId1 <= 0 || guestId2 <= 0 || guestId1 == guestId2) {
        return StatusType::INVALID_INPUT;
    }

    AVLNode<Guest, int>* node1 = guestsById.find(guestId1);
    AVLNode<Guest, int>* node2 = guestsById.find(guestId2);
    if (node1 == nullptr || node2 == nullptr) {
        return StatusType::FAILURE;
    }

    Guest* guest1 = node1->getVal();
    Guest* guest2 = node2->getVal();
    Table* table = guest2->getTable();

    if (table == nullptr || table->isFull() || guest1->getTable() != nullptr ||
        guest1->getLastEnteredMeal() == currentMeal) {
        return StatusType::FAILURE;
    }

    try {
        table->addGuest(guest1);
        guest1->setTable(table);
        guest1->setLastEnteredMeal(currentMeal);
    } catch (...) {
        return StatusType::ALLOCATION_ERROR;
    }

    return table->getId();
}

output_t<int> SegmentationResort::cleanNextRoom()
{
    if (guestsById.getSize() == 0) {
        return StatusType::FAILURE;
    }

    Guest* nextGuest = nullptr;
    if (lastCleaned == nullptr) {
        nextGuest = roomHead;
    } else {
        nextGuest = lastCleaned->nextRoom;
    }
    lastCleaned = nextGuest;
    return nextGuest->getRoom();
}
