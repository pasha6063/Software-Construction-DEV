#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ================================================================
// FORWARD DECLARATIONS
// ================================================================
class User;
class Admin;
class Seller;
class Buyer;
class Listing;
class Document;
class Bid;
class Chat;
class Message;
class Report;
class Notification;

// ================================================================
// NOTIFICATION CLASS
// Part of COMPOSITION with User
// ================================================================
class Notification
{
private:
    string notificationID;
    string message;
    string notificationType;
    string status;

public:
    // Constructor
    Notification(string id, string msg, string type)
    {
        notificationID = id;
        message = msg;
        notificationType = type;
        status = "unread";
        cout << "         [Notification] Created: " << msg << endl;
    }

    // Destructor
    ~Notification()
    {
        cout << "         [Notification] Destroyed: " << notificationID << endl;
    }

    // Methods
    void send()
    {
        status = "sent";
        cout << "         [Notification] Sent: " << message << endl;
    }

    // Getters
    string getNotificationID() { return notificationID; }
    string getMessage() { return message; }
    string getNotificationType() { return notificationType; }
    string getStatus() { return status; }

    // Setters
    void setStatus(string s) { status = s; }
};

// ================================================================
// MESSAGE CLASS
// Part of COMPOSITION with Chat
// ================================================================
class Message
{
private:
    string messageID;
    string content;
    string senderID;
    string timestamp;

public:
    // Constructor
    Message(string id, string cont, string sender, string time)
    {
        messageID = id;
        content = cont;
        senderID = sender;
        timestamp = time;
        cout << "         [Message] Created from: " << senderID << endl;
    }

    // Destructor
    ~Message()
    {
        cout << "         [Message] Destroyed: " << messageID << endl;
    }

    // Getters
    string getMessageID() { return messageID; }
    string getContent() { return content; }
    string getSenderID() { return senderID; }
    string getTimestamp() { return timestamp; }
};

// ================================================================
// DOCUMENT CLASS
// Part of COMPOSITION with Listing
// ================================================================
class Document
{
private:
    string documentID;
    string documentType;
    string fileURL;
    string verificationStatus;

public:
    // Constructor
    Document(string id, string type, string url)
    {
        documentID = id;
        documentType = type;
        fileURL = url;
        verificationStatus = "pending";
        cout << "         [Document] Created: " << documentType << endl;
    }

    // Destructor
    ~Document()
    {
        cout << "         [Document] Destroyed: " << documentID << endl;
    }

    // Getters
    string getDocumentID() { return documentID; }
    string getDocumentType() { return documentType; }
    string getFileURL() { return fileURL; }
    string getVerificationStatus() { return verificationStatus; }

    // Setters
    void setVerificationStatus(string status) { verificationStatus = status; }
};

// ================================================================
// BID CLASS
// Part of COMPOSITION with Listing (when received)
// Part of COMPOSITION with Buyer (when created)
// ================================================================
class Bid
{
private:
    string bidID;
    double amount;
    string buyerID;
    string timestamp;

public:
    // Constructor
    Bid(string id, double amt, string buyer, string time)
    {
        bidID = id;
        amount = amt;
        buyerID = buyer;
        timestamp = time;
        cout << "         [Bid] Created: $" << amount << " by " << buyerID << endl;
    }

    // Destructor
    ~Bid()
    {
        cout << "         [Bid] Destroyed: " << bidID << endl;
    }

    // Getters
    string getBidID() { return bidID; }
    double getAmount() { return amount; }
    string getBuyerID() { return buyerID; }
    string getTimestamp() { return timestamp; }
};

// ================================================================
// REPORT CLASS
// Part of COMPOSITION with Admin
// ================================================================
class Report
{
private:
    string reportID;
    string reportType;
    string data;

public:
    // Constructor
    Report(string id, string type)
    {
        reportID = id;
        reportType = type;
        data = "";
        cout << "         [Report] Created: " << reportType << endl;
    }

    // Destructor
    ~Report()
    {
        cout << "         [Report] Destroyed: " << reportID << endl;
    }

    // Methods
    void processData()
    {
        data = "Processed data for " + reportType + " report";
    }

    string exportReport(string format)
    {
        return "report_" + reportID + "." + format;
    }

    // Getters
    string getReportID() { return reportID; }
    string getReportType() { return reportType; }
    string getData() { return data; }
};

// ================================================================
// LISTING CLASS
// Part of COMPOSITION with Seller
// Has COMPOSITION with Document and Bid
// ================================================================
class Listing
{
private:
    string listingID;
    string title;
    string description;
    double price;
    string location;
    string propertyType;
    string status;
    bool isForBidding;
    double startingPrice;
    string biddingDeadline;

    // COMPOSITION: Created INSIDE this class
    vector<Document *> documents;
    vector<Bid *> bids;

public:
    // Constructor
    Listing(string id, string t, string desc, double p, string loc, string type)
    {
        listingID = id;
        title = t;
        description = desc;
        price = p;
        location = loc;
        propertyType = type;
        status = "pending";
        isForBidding = false;
        startingPrice = 0;
        biddingDeadline = "";
        cout << "      [Listing] Created: " << title << endl;
    }

    // COMPOSITION: Document created INSIDE this method
    void addDocument(string docType, string fileURL)
    {
        string docID = "DOC_" + to_string(documents.size() + 1);

        // Creating Document object INSIDE - THIS IS COMPOSITION
        Document *newDoc = new Document(docID, docType, fileURL);
        documents.push_back(newDoc);
    }

    // COMPOSITION: Bid created INSIDE this method when received
    void receiveBid(double amount, string buyerID)
    {
        if (!isForBidding)
        {
            cout << "      [Listing] Not available for bidding!" << endl;
            return;
        }
        if (amount < startingPrice)
        {
            cout << "      [Listing] Bid rejected: Below starting price" << endl;
            return;
        }

        string bidID = "BID_" + listingID + "_" + to_string(bids.size() + 1);
        string timestamp = "2025-11-15 10:00";

        // Creating Bid object INSIDE - THIS IS COMPOSITION
        Bid *newBid = new Bid(bidID, amount, buyerID, timestamp);
        bids.push_back(newBid);
    }

    // Method to set bidding
    void setForBidding(double startPrice, string deadline)
    {
        isForBidding = true;
        startingPrice = startPrice;
        biddingDeadline = deadline;
        status = "bidding";
        cout << "      [Listing] Now available for bidding. Starting: $"
             << startPrice << endl;
    }

    // Get highest bid
    Bid *getHighestBid()
    {
        if (bids.empty())
            return nullptr;

        Bid *highest = bids[0];
        for (int i = 1; i < bids.size(); i++)
        {
            if (bids[i]->getAmount() > highest->getAmount())
            {
                highest = bids[i];
            }
        }
        return highest;
    }

    // Getters
    string getListingID() { return listingID; }
    string getTitle() { return title; }
    string getDescription() { return description; }
    double getPrice() { return price; }
    string getLocation() { return location; }
    string getPropertyType() { return propertyType; }
    string getStatus() { return status; }
    bool getIsForBidding() { return isForBidding; }
    int getDocumentCount() { return documents.size(); }
    int getBidCount() { return bids.size(); }

    Document *getDocument(int index)
    {
        if (index >= 0 && index < documents.size())
        {
            return documents[index];
        }
        return nullptr;
    }

    // Setters
    void setTitle(string t) { title = t; }
    void setPrice(double p) { price = p; }
    void setStatus(string s) { status = s; }

    // Destructor - COMPOSITION: Must delete what we created
    ~Listing()
    {
        cout << "      [Listing] Destroying listing and owned objects..." << endl;

        // COMPOSITION: Delete all documents
        for (int i = 0; i < documents.size(); i++)
        {
            delete documents[i];
        }
        documents.clear();

        // COMPOSITION: Delete all bids
        for (int i = 0; i < bids.size(); i++)
        {
            delete bids[i];
        }
        bids.clear();

        cout << "      [Listing] Destroyed: " << title << endl;
    }
};

// ================================================================
// CHAT CLASS
// COMPOSITION: Creates Message objects INSIDE
// AGGREGATION: Receives Seller and Buyer pointers from OUTSIDE
// ================================================================
class Chat
{
private:
    string chatID;
    string status;

    // AGGREGATION: Passed from OUTSIDE (not created here)
    Seller *seller;
    Buyer *buyer;

    // COMPOSITION: Created INSIDE this class
    vector<Message *> messages;

public:
    // Constructor - Seller and Buyer passed from OUTSIDE (AGGREGATION)
    Chat(string id, Seller *s, Buyer *b)
    {
        chatID = id;
        seller = s; // Just storing pointer - AGGREGATION
        buyer = b;  // Just storing pointer - AGGREGATION
        status = "active";
        cout << "      [Chat] Created with ID: " << chatID << endl;
    }

    // COMPOSITION: Message created INSIDE this method
    void sendMessage(string content, string senderID)
    {
        string msgID = "MSG_" + to_string(messages.size() + 1);
        string timestamp = "2025-11-15 10:30";

        // Creating Message object INSIDE - THIS IS COMPOSITION
        Message *newMsg = new Message(msgID, content, senderID, timestamp);
        messages.push_back(newMsg);
    }

    // Getters
    string getChatID() { return chatID; }
    string getStatus() { return status; }
    int getMessageCount() { return messages.size(); }

    // Setters
    void setStatus(string s) { status = s; }

    // Destructor
    ~Chat()
    {
        cout << "      [Chat] Destroying chat and owned messages..." << endl;

        // COMPOSITION: Delete all messages (we created them)
        for (int i = 0; i < messages.size(); i++)
        {
            delete messages[i];
        }
        messages.clear();

        // AGGREGATION: Do NOT delete seller and buyer
        // Just set pointers to null, they exist independently
        seller = nullptr;
        buyer = nullptr;

        cout << "      [Chat] Destroyed: " << chatID << endl;
    }
};

// ================================================================
// USER CLASS - ABSTRACT BASE CLASS
// COMPOSITION: Creates Notification objects INSIDE
// ================================================================
class User
{
protected:
    string userID;
    string name;
    string email;
    string password;
    string accountStatus;

    // COMPOSITION: Created INSIDE this class
    vector<Notification *> notifications;

public:
    // Constructor
    User(string id, string n, string e)
    {
        userID = id;
        name = n;
        email = e;
        password = "";
        accountStatus = "pending";
    }

    // Virtual Destructor
    virtual ~User()
    {
        // COMPOSITION: Delete all notifications
        for (int i = 0; i < notifications.size(); i++)
        {
            delete notifications[i];
        }
        notifications.clear();
    }

    // Methods
    bool registerUser(string n, string e, string pass)
    {
        name = n;
        email = e;
        password = pass;
        accountStatus = "registered";
        cout << "   [User] Registered: " << name << endl;
        return true;
    }

    bool signIn(string e, string pass)
    {
        if (email == e && password == pass)
        {
            accountStatus = "active";
            cout << "   [User] Signed in: " << name << endl;
            return true;
        }
        cout << "   [User] Sign in failed for: " << e << endl;
        return false;
    }

    // COMPOSITION: Notification created INSIDE this method
    void addNotification(string message, string type)
    {
        string notifID = "NOTIF_" + to_string(notifications.size() + 1);

        // Creating Notification object INSIDE - THIS IS COMPOSITION
        Notification *newNotif = new Notification(notifID, message, type);
        newNotif->send();
        notifications.push_back(newNotif);
    }

    // Getters
    string getUserID() { return userID; }
    string getName() { return name; }
    string getEmail() { return email; }
    string getAccountStatus() { return accountStatus; }
    int getNotificationCount() { return notifications.size(); }

    // Setters
    void setName(string n) { name = n; }
    void setEmail(string e) { email = e; }
    void setPassword(string p) { password = p; }
    void setAccountStatus(string s) { accountStatus = s; }
};

// ================================================================
// ADMIN CLASS - Inherits from User
// COMPOSITION: Creates Report objects INSIDE
// ASSOCIATION: Uses Listing and Chat objects (doesn't own them)
// ================================================================
class Admin : public User
{
private:
    string adminLevel;

    // COMPOSITION: Created INSIDE this class
    vector<Report *> reports;

public:
    // Constructor
    Admin(string id, string name, string email, string level)
        : User(id, name, email)
    {
        adminLevel = level;
        cout << "   [Admin] Created: " << name << endl;
    }

    // ASSOCIATION: Uses User object (doesn't own it)
    bool manageUserAccount(string action, User *user)
    {
        if (user == nullptr)
            return false;

        if (action == "activate")
        {
            user->setAccountStatus("active");
            cout << "   [Admin] Activated user: " << user->getName() << endl;
        }
        else if (action == "block")
        {
            user->setAccountStatus("blocked");
            cout << "   [Admin] Blocked user: " << user->getName() << endl;
        }
        else if (action == "delete")
        {
            user->setAccountStatus("deleted");
            cout << "   [Admin] Deleted user: " << user->getName() << endl;
        }
        return true;
    }

    // ASSOCIATION: Uses Listing object (doesn't own it)
    bool approveListing(Listing *listing)
    {
        if (listing == nullptr)
            return false;
        listing->setStatus("approved");
        cout << "   [Admin] Approved listing: " << listing->getTitle() << endl;
        return true;
    }

    // ASSOCIATION: Uses Listing object (doesn't own it)
    bool rejectListing(Listing *listing)
    {
        if (listing == nullptr)
            return false;
        listing->setStatus("rejected");
        cout << "   [Admin] Rejected listing: " << listing->getTitle() << endl;
        return true;
    }

    // ASSOCIATION: Uses Listing object for document verification
    bool verifyDocuments(Listing *listing)
    {
        if (listing == nullptr)
            return false;
        if (listing->getDocumentCount() == 0)
        {
            cout << "   [Admin] No documents to verify for: " << listing->getTitle() << endl;
            return false;
        }

        // Verify each document
        for (int i = 0; i < listing->getDocumentCount(); i++)
        {
            Document *doc = listing->getDocument(i);
            if (doc != nullptr)
            {
                doc->setVerificationStatus("verified");
                cout << "   [Admin] Verified document: " << doc->getDocumentType() << endl;
            }
        }
        listing->setStatus("verified");
        return true;
    }

    // ASSOCIATION: Uses Listing object to monitor bidding
    void monitorBidding(Listing *listing)
    {
        if (listing == nullptr)
            return;

        cout << "   [Admin] Monitoring bids for: " << listing->getTitle() << endl;
        cout << "   [Admin] Total bids: " << listing->getBidCount() << endl;

        Bid *highest = listing->getHighestBid();
        if (highest != nullptr)
        {
            cout << "   [Admin] Highest bid: $" << highest->getAmount()
                 << " by " << highest->getBuyerID() << endl;
        }
    }

    // ASSOCIATION: Uses Chat object (doesn't own it)
    void moderateChat(Chat *chat)
    {
        if (chat == nullptr)
            return;
        cout << "   [Admin] Moderating chat: " << chat->getChatID() << endl;
        cout << "   [Admin] Messages in chat: " << chat->getMessageCount() << endl;
    }

    // COMPOSITION: Report created INSIDE this method
    void generateReport(string type)
    {
        string reportID = "RPT_" + to_string(reports.size() + 1);

        // Creating Report object INSIDE - THIS IS COMPOSITION
        Report *newReport = new Report(reportID, type);
        newReport->processData();
        reports.push_back(newReport);

        cout << "   [Admin] Generated report: " << type << endl;
    }

    // Getters
    string getAdminLevel() { return adminLevel; }
    int getReportCount() { return reports.size(); }

    // Setters
    void setAdminLevel(string level) { adminLevel = level; }

    // Destructor - COMPOSITION: Must delete what we created
    ~Admin()
    {
        cout << "   [Admin] Destroying admin and owned objects..." << endl;

        // COMPOSITION: Delete all reports
        for (int i = 0; i < reports.size(); i++)
        {
            delete reports[i];
        }
        reports.clear();

        cout << "   [Admin] Destroyed: " << name << endl;
    }
};

// ================================================================
// SELLER CLASS - Inherits from User
// COMPOSITION: Creates Listing objects INSIDE
// ================================================================
class Seller : public User
{
private:
    // COMPOSITION: Created INSIDE this class
    vector<Listing *> listings;

public:
    // Constructor
    Seller(string id, string name, string email)
        : User(id, name, email)
    {
        cout << "   [Seller] Created: " << name << endl;
    }

    // COMPOSITION: Listing created INSIDE this method
    void createListing(string title, string desc, double price,
                       string location, string propertyType)
    {
        string listingID = "LST_" + userID + "_" + to_string(listings.size() + 1);

        // Creating Listing object INSIDE - THIS IS COMPOSITION
        Listing *newListing = new Listing(listingID, title, desc, price,
                                          location, propertyType);
        listings.push_back(newListing);
    }

    // Edit a listing
    bool editListing(int index, string newTitle, double newPrice)
    {
        if (index < 0 || index >= listings.size())
            return false;

        listings[index]->setTitle(newTitle);
        listings[index]->setPrice(newPrice);
        cout << "   [Seller] Edited listing: " << newTitle << endl;
        return true;
    }

    // Delete a listing
    bool deleteListing(int index)
    {
        if (index < 0 || index >= listings.size())
            return false;

        cout << "   [Seller] Deleting listing: " << listings[index]->getTitle() << endl;
        delete listings[index];
        listings.erase(listings.begin() + index);
        return true;
    }

    // Upload document to a listing
    void uploadDocument(Listing *listing, string docType, string fileURL)
    {
        if (listing != nullptr)
        {
            listing->addDocument(docType, fileURL);
            cout << "   [Seller] Uploaded document to: " << listing->getTitle() << endl;
        }
    }

    // Post listing for bidding
    bool postForBidding(Listing *listing, double startPrice, string deadline)
    {
        if (listing == nullptr)
            return false;
        if (listing->getStatus() != "approved" && listing->getStatus() != "verified")
        {
            cout << "   [Seller] Listing must be approved first!" << endl;
            return false;
        }

        listing->setForBidding(startPrice, deadline);
        cout << "   [Seller] Posted for bidding: " << listing->getTitle() << endl;
        return true;
    }

    // Get a listing (for other classes to use - ASSOCIATION)
    Listing *getListing(int index)
    {
        if (index >= 0 && index < listings.size())
        {
            return listings[index];
        }
        return nullptr;
    }

    // Getters
    int getListingCount() { return listings.size(); }

    // Destructor - COMPOSITION: Must delete what we created
    ~Seller()
    {
        cout << "   [Seller] Destroying seller and owned objects..." << endl;

        // COMPOSITION: Delete all listings
        for (int i = 0; i < listings.size(); i++)
        {
            delete listings[i];
        }
        listings.clear();

        cout << "   [Seller] Destroyed: " << name << endl;
    }
};

// ================================================================
// BUYER CLASS - Inherits from User
// COMPOSITION: Creates Bid objects INSIDE
// Creates Chat objects (which store participants via AGGREGATION)
// ================================================================
class Buyer : public User
{
private:
    // COMPOSITION: Created INSIDE this class
    vector<Bid *> bids;

public:
    // Constructor
    Buyer(string id, string name, string email)
        : User(id, name, email)
    {
        cout << "   [Buyer] Created: " << name << endl;
    }

    // ASSOCIATION: Just uses Listing to search
    void searchListings(string keyword)
    {
        cout << "   [Buyer] Searching for: " << keyword << endl;
    }

    // ASSOCIATION: Just uses Listing to filter
    void filterListings(string location, double minPrice, double maxPrice)
    {
        cout << "   [Buyer] Filtering - Location: " << location
             << ", Price: $" << minPrice << " - $" << maxPrice << endl;
    }

    // Place bid on a listing
    // COMPOSITION: Bid created INSIDE and stored in buyer
    // Listing also receives a copy of bid info
    bool placeBid(Listing *listing, double amount)
    {
        if (listing == nullptr)
            return false;
        if (!listing->getIsForBidding())
        {
            cout << "   [Buyer] Listing not available for bidding!" << endl;
            return false;
        }

        // Create bid and store in buyer's bid list
        string bidID = "BID_" + userID + "_" + to_string(bids.size() + 1);
        string timestamp = "2025-11-15 11:00";

        // Creating Bid object INSIDE - THIS IS COMPOSITION
        Bid *newBid = new Bid(bidID, amount, userID, timestamp);
        bids.push_back(newBid);

        // Also notify the listing (listing creates its own record)
        listing->receiveBid(amount, userID);

        cout << "   [Buyer] Placed bid: $" << amount << " on "
             << listing->getTitle() << endl;
        return true;
    }

    // Create chat with seller
    // Returns Chat object - Chat has AGGREGATION with Seller and Buyer
    Chat *initiateChatWithSeller(Seller *seller)
    {
        if (seller == nullptr)
            return nullptr;

        string chatID = "CHAT_" + userID + "_" + seller->getUserID();

        // Creating Chat - Chat receives Seller and Buyer as AGGREGATION
        Chat *newChat = new Chat(chatID, seller, this);

        cout << "   [Buyer] Initiated chat with: " << seller->getName() << endl;
        return newChat;
    }

    // Getters
    int getBidCount() { return bids.size(); }

    // Destructor - COMPOSITION: Must delete what we created
    ~Buyer()
    {
        cout << "   [Buyer] Destroying buyer and owned objects..." << endl;

        // COMPOSITION: Delete all bids
        for (int i = 0; i < bids.size(); i++)
        {
            delete bids[i];
        }
        bids.clear();

        cout << "   [Buyer] Destroyed: " << name << endl;
    }
};

// ================================================================
// MAIN FUNCTION - Demonstrates All Relationships
// ================================================================
int main()
{
    cout << "============================================================" << endl;
    cout << "      PROPERTY BIDDING SYSTEM - ALL RELATIONSHIPS           " << endl;
    cout << "============================================================" << endl;
    cout << endl;

    // --------------------------------------------------------
    // 1. INHERITANCE: All user types extend User
    // --------------------------------------------------------
    cout << "1. INHERITANCE (User <|-- Admin, Seller, Buyer)" << endl;
    cout << "   All child classes inherit User attributes and methods" << endl;
    cout << "   ----------------------------------------------------" << endl;

    Admin *admin = new Admin("A001", "AdminUser", "admin@property.com", "super");
    Seller *seller = new Seller("S001", "Ali Khan", "ali@property.com");
    Buyer *buyer = new Buyer("B001", "Ahmed Hassan", "ahmed@property.com");

    // Using inherited methods
    admin->setPassword("admin123");
    admin->signIn("admin@property.com", "admin123");

    seller->setPassword("seller123");
    seller->signIn("ali@property.com", "seller123");

    buyer->setPassword("buyer123");
    buyer->signIn("ahmed@property.com", "buyer123");
    cout << endl;

    // --------------------------------------------------------
    // 2. COMPOSITION: Seller creates Listing INSIDE
    // --------------------------------------------------------
    cout << "2. COMPOSITION (Seller *-- Listing)" << endl;
    cout << "   Listing objects created INSIDE Seller methods" << endl;
    cout << "   ----------------------------------------------------" << endl;

    seller->createListing("3 Bedroom House", "Beautiful house in DHA",
                          250000, "Lahore", "House");
    seller->createListing("2 Bedroom Apartment", "Modern apartment in Gulberg",
                          150000, "Lahore", "Apartment");

    cout << "   Seller owns " << seller->getListingCount() << " listings" << endl;
    cout << endl;

    // --------------------------------------------------------
    // 3. COMPOSITION: Listing creates Document INSIDE
    // --------------------------------------------------------
    cout << "3. COMPOSITION (Listing *-- Document)" << endl;
    cout << "   Document objects created INSIDE Listing methods" << endl;
    cout << "   ----------------------------------------------------" << endl;

    Listing *listing1 = seller->getListing(0);
    Listing *listing2 = seller->getListing(1);

    // Upload documents to listings
    seller->uploadDocument(listing1, "Ownership Certificate", "doc1.pdf");
    seller->uploadDocument(listing1, "CNIC Copy", "doc2.pdf");
    seller->uploadDocument(listing2, "Ownership Certificate", "doc3.pdf");

    cout << "   Listing 1 has " << listing1->getDocumentCount() << " documents" << endl;
    cout << "   Listing 2 has " << listing2->getDocumentCount() << " documents" << endl;
    cout << endl;

    // --------------------------------------------------------
    // 4. ASSOCIATION: Admin uses Listing (approve/verify)
    // --------------------------------------------------------
    cout << "4. ASSOCIATION (Admin --> Listing)" << endl;
    cout << "   Admin uses Listing objects without owning them" << endl;
    cout << "   ----------------------------------------------------" << endl;

    admin->approveListing(listing1);
    admin->verifyDocuments(listing1);
    admin->approveListing(listing2);
    cout << endl;

    // --------------------------------------------------------
    // 5. COMPOSITION: Post for Bidding (Listing creates Bid INSIDE)
    // --------------------------------------------------------
    cout << "5. COMPOSITION (Listing *-- Bid when receiving bids)" << endl;
    cout << "   Bid objects created INSIDE Listing methods" << endl;
    cout << "   ----------------------------------------------------" << endl;

    seller->postForBidding(listing1, 200000, "2025-12-31");
    cout << endl;

    // --------------------------------------------------------
    // 6. COMPOSITION: Buyer creates Bid INSIDE
    // --------------------------------------------------------
    cout << "6. COMPOSITION (Buyer *-- Bid)" << endl;
    cout << "   Buyer creates Bid objects INSIDE placeBid method" << endl;
    cout << "   ----------------------------------------------------" << endl;

    // Create another buyer for bidding
    Buyer *buyer2 = new Buyer("B002", "Sara Ali", "sara@property.com");
    buyer2->setPassword("buyer456");
    buyer2->signIn("sara@property.com", "buyer456");

    buyer->placeBid(listing1, 220000);
    buyer2->placeBid(listing1, 235000);
    buyer->placeBid(listing1, 250000);

    cout << "   Buyer 1 has " << buyer->getBidCount() << " bids" << endl;
    cout << "   Buyer 2 has " << buyer2->getBidCount() << " bids" << endl;
    cout << "   Listing has " << listing1->getBidCount() << " bids" << endl;
    cout << endl;

    // --------------------------------------------------------
    // 7. Admin monitors bidding (ASSOCIATION)
    // --------------------------------------------------------
    cout << "7. ADMIN MONITORS BIDDING (Association)" << endl;
    cout << "   ----------------------------------------------------" << endl;

    admin->monitorBidding(listing1);
    cout << endl;

    // --------------------------------------------------------
    // 8. AGGREGATION + COMPOSITION: Chat
    //    Chat receives Seller/Buyer (AGGREGATION)
    //    Chat creates Message (COMPOSITION)
    // --------------------------------------------------------
    cout << "8. CHAT: Aggregation (Seller, Buyer) + Composition (Message)" << endl;
    cout << "   Chat receives participants from OUTSIDE (Aggregation)" << endl;
    cout << "   Chat creates Messages INSIDE (Composition)" << endl;
    cout << "   ----------------------------------------------------" << endl;

    // Buyer initiates chat with Seller
    // Chat receives Seller and Buyer pointers (AGGREGATION)
    Chat *chat = buyer->initiateChatWithSeller(seller);

    // Messages created INSIDE Chat (COMPOSITION)
    chat->sendMessage("Hi, I am interested in the house", buyer->getUserID());
    chat->sendMessage("Great! Would you like to schedule a visit?", seller->getUserID());
    chat->sendMessage("Yes, tomorrow at 3 PM works for me", buyer->getUserID());

    cout << "   Chat has " << chat->getMessageCount() << " messages" << endl;
    cout << endl;

    // --------------------------------------------------------
    // 9. Admin moderates chat (ASSOCIATION)
    // --------------------------------------------------------
    cout << "9. ADMIN MODERATES CHAT (Association)" << endl;
    cout << "   Admin uses Chat object without owning it" << endl;
    cout << "   ----------------------------------------------------" << endl;

    admin->moderateChat(chat);
    cout << endl;

    // --------------------------------------------------------
    // 10. COMPOSITION: Admin creates Report INSIDE
    // --------------------------------------------------------
    cout << "10. COMPOSITION (Admin *-- Report)" << endl;
    cout << "    Report objects created INSIDE Admin methods" << endl;
    cout << "    ----------------------------------------------------" << endl;

    admin->generateReport("User Activity");
    admin->generateReport("Listing Statistics");
    admin->generateReport("Bidding Summary");

    cout << "    Admin owns " << admin->getReportCount() << " reports" << endl;
    cout << endl;

    // --------------------------------------------------------
    // 11. COMPOSITION: User creates Notification INSIDE
    // --------------------------------------------------------
    cout << "11. COMPOSITION (User *-- Notification)" << endl;
    cout << "    Notification objects created INSIDE User methods" << endl;
    cout << "    ----------------------------------------------------" << endl;

    seller->addNotification("Your listing has been approved", "approval");
    buyer->addNotification("You have been outbid on House listing", "bid");
    buyer->addNotification("New message from seller", "message");

    cout << "    Seller has " << seller->getNotificationCount() << " notifications" << endl;
    cout << "    Buyer has " << buyer->getNotificationCount() << " notifications" << endl;
    cout << endl;

    // --------------------------------------------------------
    // 12. CLEANUP - Demonstrates Composition destruction
    // --------------------------------------------------------
    cout << "12. CLEANUP - Composition destroys owned objects" << endl;
    cout << "    ----------------------------------------------------" << endl;

    cout << endl
         << "    Deleting Chat (Messages destroyed)..." << endl;
    delete chat; // Messages destroyed (Composition)
                 // Seller and Buyer NOT destroyed (Aggregation)

    cout << endl
         << "    Deleting Buyer2 (Bids destroyed)..." << endl;
    delete buyer2; // Bids destroyed (Composition)

    cout << endl
         << "    Deleting Buyer (Bids destroyed)..." << endl;
    delete buyer; // Bids destroyed (Composition)

    cout << endl
         << "    Deleting Seller (Listings->Documents->Bids destroyed)..." << endl;
    delete seller; // Listings, Documents, Bids destroyed (Composition chain)

    cout << endl
         << "    Deleting Admin (Reports destroyed)..." << endl;
    delete admin; // Reports destroyed (Composition)

    cout << endl;
    cout << "============================================================" << endl;
    cout << "                  PROGRAM COMPLETED                         " << endl;
    cout << "============================================================" << endl;

    return 0;
}