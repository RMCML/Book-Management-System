#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/time.h>


#define MAX_BORROW_NUM 2 //读者最大借阅量
#define BOOK_NAME_LEN 100 //书名最大长度
#define AUTHOR_NAME_LEN 50 //作者名字最大长度
#define READER_NAME_LEN 50 //读者姓名最大长度


enum Status
{
    // 内存分配错误
    MemoryError = 1,
    // 录入成功
    Succeed,
    // 取消录入
    CancelInput,
    // 重复录入
    RepeatInput,

};

// 书籍信息
typedef struct bookNode
{
    // 书籍ID
    int ID;
    // 书籍链表长度
    int Length;
    // 书籍库存
    int Inventory;
    // 书籍借阅量
    int BorrowedNum;
    // 头节点
    bool IsHead;

    char Name[BOOK_NAME_LEN];
    char Author[AUTHOR_NAME_LEN];

    struct bookNode *Next;
}Book, *PtrBook;

// 借阅记录
typedef struct borrowRecord
{
    int BookID;

    char BookName[BOOK_NAME_LEN];

    // Unix 时间戳
    // 借阅时间戳
    time_t BorrowTime;
    // 归还时间戳
    time_t ReturnTime;

}BorrowRecord, *PtrBorrowRecord;

// 读者
typedef struct borrowerNode
{
    int ID;
    // 头节点
    bool IsHead;
    // 借阅者链表长度
    int Length;
    // 读者姓名
    char Name[READER_NAME_LEN];
    // 借阅读书记录
    BorrowRecord BorrowingRecord[MAX_BORROW_NUM];
    // 当前借阅图书量
    int CurrentBorrowingNum;
    // 是否可以借阅
    bool CanBorrow;

    struct borrowerNode *Next;
}Borrower, *PtrBorrower;

// Function declaration
bool IsBookNodeEmpty(PtrBook BookNode);
bool IsBorrowerNodeEmpty(PtrBorrower BorrowerNode);
bool IsBorrowRecordEmpty(PtrBorrowRecord BorrowRecordNode);
time_t GetTimestamp();
struct tm TimestampToUTC(time_t timestamp);
struct tm TimestampToLocal(time_t timestamp);
time_t UTCToTimestamp(struct tm structTime);
enum Status CreateBookList(PtrBook PtrBookHead);
enum Status InsertBook(PtrBook PtrBookHead);
bool IsBookExists(PtrBook PtrBookHead, int bookID);
void DeepCopyBookNode(PtrBook PtrBookTargetNode, PtrBook PtrBookSourceNode);
PtrBook FindBookByID(PtrBook PtrBookHead, int bookID);
PtrBook FindBookByName(PtrBook PtrBookHead, char *bookName);
PtrBook FindBookByIndex(PtrBook PtrBookHead, int index);
void PrintBookNode(PtrBook PtrBookNode);
void PrintBookList(PtrBook PtrBookHead);
enum Status CreateBorrowerList(PtrBorrower PtrBorrowerHead);
enum Status InsertBorrower(PtrBorrower PtrBorrowerHead);
bool IsBorrowerExists(PtrBorrower PtrBorrowerHead, int borrowerID);
void DeepCopyBorrowerNode(PtrBorrower PtrBorrowerTargetNode, PtrBorrower PtrBorrowerSourceNode);
PtrBorrower FindBorrowerByID(PtrBorrower PtrBorrowerHead, int borrowerID);
PtrBorrower FindBorrowerByName(PtrBorrower PtrBorrowerHead, char *borrowerName);
void PrintfBorrowRecordNode(BorrowRecord borrowRecordNode);
void PrintBorrowerNode(PtrBorrower PtrBorrowerNode);
void PrintBorrowerList(PtrBorrower PtrBorrowerHead);
void QueryBook(PtrBook PtrBookHead);
void QueryBorrower(PtrBorrower PtrBorrowerHead);
bool IsBookAllowBorrow(PtrBook PtrBookNode);
bool IsBorrowerAllowBorrow(PtrBorrower PtrBorrowerNode);
bool IsAllowBorrow(PtrBook PtrBookNode, PtrBorrower PtrBorrowerNode);
void AssignedBookToBorrower(PtrBook PtrBookNode, PtrBorrower PtrBorrowerNode);
void BorrowBook(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead);
void ReturnBook(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead);
void BookManagement(PtrBook PtrBookHead);
void BorrowerManagement(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead);
void ReleaseBookList(PtrBook PtrBookHead);
void ReleaseBorrowerList(PtrBorrower PtrBorrowerHead);


// 书籍节点是否为空
bool IsBookNodeEmpty(PtrBook BookNode)
{
    return BookNode ? false : true;
}

// 读者节点是否为空
bool IsBorrowerNodeEmpty(PtrBorrower BorrowerNode)
{
    return BorrowerNode ? false : true;
}

// 借阅记录节点是否为空
bool IsBorrowRecordEmpty(PtrBorrowRecord BorrowRecordNode)
{
    return BorrowRecordNode ? false : true;
}

// 获取当前时间戳
time_t GetTimestamp()
{
    time_t  timeNow;
    time(&timeNow);
    return timeNow;
}

// 时间戳转UTC时间
struct tm TimestampToUTC(time_t timestamp)
{
    return *gmtime(&timestamp);
}

// 时间戳转当地时间
struct tm TimestampToLocal(time_t timestamp)
{
    struct tm structTime;

    //获取时区信息
    tzset();

    structTime = TimestampToUTC(timestamp);
    structTime.tm_sec -= timezone;
    // 修正tm结构
    mktime(&structTime);

    return structTime;
}

// UTC时间转时间戳
time_t UTCToTimestamp(struct tm structTime)
{
    return mktime(&structTime);
}

// 首次录入书籍时创建书籍链表，并返回链表头指针
enum Status CreateBookList(PtrBook PtrBookHead)
{
    PtrBook PtrBookCur = (PtrBook) malloc(sizeof(Book));

    if(!IsBookNodeEmpty(PtrBookCur))
    {
        printf("\r\n请根据提示输入图书的相关信息：\r\n录入格式(以单个空格分隔)：书号 书名 作者 库存");
        scanf("%d %s %s %d",
              &PtrBookCur->ID, PtrBookCur->Name, PtrBookCur->Author, &PtrBookCur->Inventory);

        PtrBookCur->BorrowedNum = 0;
        PtrBookCur->IsHead = false;
        PtrBookCur->Next = NULL;

        PtrBookHead->Next = PtrBookCur;
        PtrBookHead->Length++;
        printf("图书录入完毕");
    }
    else
    {
        printf("录入失败，分配内存不足");
        return MemoryError;
    }

    return Succeed;
}

// 新增录入书籍,因未实现双向链表，选择尾插法
// 如经过排序，则可实现多种查询算法,且保持插入有序
enum Status InsertBook(PtrBook PtrBookHead)
{
    PtrBook PtrBookPrev = PtrBookHead;
    // 当前节点（这里为第一个数据节点）
    PtrBook PtrBookCur = PtrBookHead->Next;
    PtrBook PtrBookNode = (PtrBook) malloc(sizeof(Book));
    if(!IsBookNodeEmpty(PtrBookNode))
    {
        printf("\r\n!!!请根据提示输入图书的相关信息：\r\n录入格式(以单个空格分隔)：书号 书名 作者 库存");
        scanf("%d %s %s %d",
              &PtrBookNode->ID, PtrBookNode->Name, PtrBookNode->Author, &PtrBookNode->Inventory);

        if(IsBookExists(PtrBookHead, PtrBookNode->ID))
        {
            return RepeatInput;
        }

        PtrBookNode->IsHead = false;
        PtrBookNode->BorrowedNum = 0;
        while(!IsBookNodeEmpty(PtrBookCur) && PtrBookCur->ID < PtrBookNode->ID)
        {
            PtrBookPrev = PtrBookCur;
            PtrBookCur = PtrBookCur->Next;
        }

        PtrBookNode->Next = PtrBookCur;
        PtrBookPrev->Next = PtrBookNode;
        PtrBookHead->Length++;
        printf("图书录入完毕");
    }
    else
    {
        printf("分配内存失败");
        exit(0);
        // return MemoryError;
    }
    return Succeed;
}

// 根据书籍唯一ID判断是否已录入
bool IsBookExists(PtrBook PtrBookHead, int bookID)
{
    return FindBookByID(PtrBookHead, bookID) ? true : false;
}

// 对书籍链表节点进行深拷贝
void DeepCopyBookNode(PtrBook PtrBookTargetNode, PtrBook PtrBookSourceNode)
{
    PtrBookTargetNode->ID = PtrBookSourceNode->ID;
    PtrBookTargetNode->BorrowedNum = PtrBookSourceNode->BorrowedNum;
    strcpy(PtrBookTargetNode->Name, PtrBookSourceNode->Name);
    PtrBookTargetNode->Inventory = PtrBookSourceNode->Inventory;
    PtrBookTargetNode->IsHead = PtrBookSourceNode->IsHead;
    strcpy(PtrBookTargetNode->Author, PtrBookSourceNode->Author);
}

// 根据书籍唯一ID，查询书籍并返回该节点
// 等于根据节点值查找节点位置
PtrBook FindBookByID(PtrBook PtrBookHead, int bookID)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    if (IsBookNodeEmpty(PtrBookCur))
    {
        printf("未录入书籍");
    }
    else
    {
        while (!IsBookNodeEmpty(PtrBookCur->Next) && PtrBookCur->ID != bookID)
        {
            PtrBookCur = PtrBookCur->Next;
        }

        if(PtrBookCur->ID != bookID)
        {
            PtrBookCur = NULL;
        }
    }

    return PtrBookCur;
}

// 根据书名查询书籍，返回带头节点的查询结果链表
// 返回的是头指针

// 入参  第一个书籍链表头指针 第二个 书名
// 返回 查询到的书籍链表
PtrBook FindBookByName(PtrBook PtrBookHead, char *bookName)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    PtrBook PtrBookResultHead = (PtrBook) malloc(sizeof(Book));
    PtrBook PtrBookResultTail, PtrBookResultCur;
    PtrBookResultTail = PtrBookResultHead;

    if(IsBookNodeEmpty(PtrBookResultHead))
    {
        printf("分配内存失败");
        exit(0);
    }
    else
    {
        PtrBookResultHead->Length = 0;
        PtrBookResultHead->IsHead = true;
        PtrBookResultHead->Next = NULL;
        while (!IsBookNodeEmpty(PtrBookCur))
        {
            if(strcmp(PtrBookCur->Name, bookName) == 0)
            {
                PtrBookResultCur = (PtrBook) malloc(sizeof(Book));
                if(IsBookNodeEmpty(PtrBookResultCur))
                {
                    printf("分配内存失败");
                    exit(0);
                }
                else
                {
                    // 尾插法
                    DeepCopyBookNode(PtrBookResultCur, PtrBookCur);

                    PtrBookResultCur->Next = NULL;
                    PtrBookResultTail->Next = PtrBookResultCur;
                    PtrBookResultTail = PtrBookResultCur;
                    PtrBookResultHead->Length++;
                }
            }

            PtrBookCur = PtrBookCur->Next;
        }
    }

    return PtrBookResultHead;
}

// 根据书籍节点位置查询书籍，返回该节点
PtrBook FindBookByIndex(PtrBook PtrBookHead, int index)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    int currentPosition = 1;
    if (IsBookNodeEmpty(PtrBookCur))
    {
        printf("未录入书籍");
    }
    else
    {
        while (!IsBookNodeEmpty(PtrBookCur->Next) && currentPosition != index)
        {
            PtrBookCur = PtrBookCur->Next;
            currentPosition++;
        }

        if(currentPosition != index)
        {
            PtrBookCur = NULL;
        }
    }

    return PtrBookCur;
}

// 输出书籍节点
void PrintBookNode(PtrBook PtrBookNode)
{
    printf("书籍ID : %d\r\n书籍库存 : %d\r\n书籍借阅量 : %d\r\n书名 : %s\r\n书籍作者 : %s\r\n",
           PtrBookNode->ID, PtrBookNode->Inventory, PtrBookNode->BorrowedNum,
           PtrBookNode->Name, PtrBookNode->Author);
}

// 输出书籍链表，顺序输出
void PrintBookList(PtrBook PtrBookHead)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    if(IsBookNodeEmpty(PtrBookCur))
    {
        printf("书籍链表为空");
    }
    else
    {
        while (!IsBookNodeEmpty(PtrBookCur))
        {
            PrintBookNode(PtrBookCur);
            PtrBookCur = PtrBookCur->Next;
        }
    }
}

// 首次录入读者时创建读者链表，并返回链表头指针
enum Status CreateBorrowerList(PtrBorrower PtrBorrowerHead)
{
    PtrBorrower PtrBorrowerCur = (PtrBorrower) malloc(sizeof(Borrower));

    if(!IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("\r\n请根据提示输入借阅人的相关信息：\r\n录入格式(以单个空格分隔)：编号 姓名");
        scanf("%d %s", &PtrBorrowerCur->ID, PtrBorrowerCur->Name);

        PtrBorrowerCur->CurrentBorrowingNum = 0;
        PtrBorrowerCur->IsHead = false;
        PtrBorrowerCur->CanBorrow = true;
        PtrBorrowerCur->Next = NULL;
        PtrBorrowerHead->Next = PtrBorrowerCur;
        PtrBorrowerHead->Length++;
        printf("读者信息录入完毕");
    }
    else
    {
        printf("录入失败，分配内存不足");
        return MemoryError;
    }

    return Succeed;
}

// 新增读者
enum Status InsertBorrower(PtrBorrower PtrBorrowerHead)
{
    PtrBorrower PtrBorrowerPrev = PtrBorrowerHead;
    // 当前节点（这里为第一个数据节点）
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    PtrBorrower PtrBorrowerNode = (PtrBorrower) malloc(sizeof(Borrower));
    if(!IsBorrowerNodeEmpty(PtrBorrowerNode))
    {
        printf("\r\n请根据提示输入读者的相关信息：\r\n录入格式(以单个空格分隔)：编号 姓名");
        scanf("%d %s", &PtrBorrowerNode->ID, PtrBorrowerNode->Name);

        if(IsBorrowerExists(PtrBorrowerHead, PtrBorrowerNode->ID))
        {
            return RepeatInput;
        }

        PtrBorrowerNode->IsHead = false;
        PtrBorrowerNode->CanBorrow = true;
        PtrBorrowerNode->CurrentBorrowingNum = 0;
        while(!IsBorrowerNodeEmpty(PtrBorrowerCur) && PtrBorrowerCur->ID < PtrBorrowerNode->ID)
        {
            PtrBorrowerPrev = PtrBorrowerCur;
            PtrBorrowerCur = PtrBorrowerCur->Next;
        }

        PtrBorrowerNode->Next = PtrBorrowerCur;
        PtrBorrowerPrev->Next = PtrBorrowerNode;
        PtrBorrowerHead->Length++;
        printf("读者信息录入完毕");
    }
    else
    {
        printf("分配内存失败");
        exit(0);
        // return MemoryError;
    }
    return Succeed;
}

// 根据读者唯一ID判断是否已录入
bool IsBorrowerExists(PtrBorrower PtrBorrowerHead, int borrowerID)
{
    return FindBorrowerByID(PtrBorrowerHead, borrowerID) ? true : false;
}

// 对书籍链表节点进行深拷贝
void DeepCopyBorrowerNode(PtrBorrower PtrBorrowerTargetNode, PtrBorrower PtrBorrowerSourceNode)
{
    PtrBorrowerTargetNode->ID = PtrBorrowerSourceNode->ID;
    strcpy(PtrBorrowerTargetNode->Name, PtrBorrowerSourceNode->Name);
    PtrBorrowerTargetNode->CanBorrow = PtrBorrowerSourceNode->CanBorrow;
    PtrBorrowerTargetNode->IsHead = PtrBorrowerSourceNode->IsHead;
    PtrBorrowerTargetNode->CurrentBorrowingNum = PtrBorrowerSourceNode->CurrentBorrowingNum;
    for(int i = 0; i < PtrBorrowerSourceNode->CurrentBorrowingNum; i++)
    {
        PtrBorrowerTargetNode->BorrowingRecord[i].BookID = PtrBorrowerSourceNode->BorrowingRecord[i].BookID;
        PtrBorrowerTargetNode->BorrowingRecord[i].BorrowTime = PtrBorrowerSourceNode->BorrowingRecord[i].BorrowTime;
        PtrBorrowerTargetNode->BorrowingRecord[i].ReturnTime = PtrBorrowerSourceNode->BorrowingRecord[i].ReturnTime;
        strcpy(PtrBorrowerTargetNode->BorrowingRecord[i].BookName, PtrBorrowerSourceNode->BorrowingRecord[i].BookName);
    }
}

// 根据读者唯一ID，查询读者并返回该节点
// 等于根据节点值查找节点位置
PtrBorrower FindBorrowerByID(PtrBorrower PtrBorrowerHead, int borrowerID)
{
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    if (IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("该读者未录入");
    }
    else
    {
        while (!IsBorrowerNodeEmpty(PtrBorrowerCur->Next) && PtrBorrowerCur->ID != borrowerID)
        {
            PtrBorrowerCur = PtrBorrowerCur->Next;
        }

        if(PtrBorrowerCur->ID != borrowerID)
        {
            PtrBorrowerCur = NULL;
        }
    }

    return PtrBorrowerCur;
}

// 根据名字查询读者，返回带头节点的查询结果链表，
// 返回的是头指针
PtrBorrower FindBorrowerByName(PtrBorrower PtrBorrowerHead, char *borrowerName)
{
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    PtrBorrower PtrBorrowerResultHead = (PtrBorrower) malloc(sizeof(Borrower));
    PtrBorrower PtrBorrowerResultTail, PtrBorrowerResultCur;
    PtrBorrowerResultTail = PtrBorrowerResultHead;

    if(IsBorrowerNodeEmpty(PtrBorrowerResultHead))
    {
        printf("分配内存失败");
        exit(0);
    }
    else
    {
        PtrBorrowerResultHead->Length = 0;
        PtrBorrowerResultHead->IsHead = true;
        PtrBorrowerResultHead->Next = NULL;
        while (!IsBorrowerNodeEmpty(PtrBorrowerCur))
        {
            if(strcmp(PtrBorrowerCur->Name, borrowerName) == 0)
            {
                PtrBorrowerResultCur = (PtrBorrower) malloc(sizeof(Borrower));
                if(IsBorrowerNodeEmpty(PtrBorrowerResultCur))
                {
                    printf("分配内存失败");
                    exit(0);
                }
                else
                {
                    // 尾插法
                    DeepCopyBorrowerNode(PtrBorrowerResultCur, PtrBorrowerCur);
                    PtrBorrowerResultCur->Next = NULL;
                    PtrBorrowerResultTail->Next = PtrBorrowerResultCur;
                    PtrBorrowerResultTail = PtrBorrowerResultCur;
                    PtrBorrowerResultHead->Length++;
                }
            }

            PtrBorrowerCur = PtrBorrowerCur->Next;
        }
    }

    return PtrBorrowerResultHead;
}

// 输出借阅记录
void PrintfBorrowRecordNode(BorrowRecord borrowRecordNode)
{
    char borrowTimeBuff[100], returnTimeBuff[100];
    struct tm structBorrowTime, structReturnTime;

    structBorrowTime = TimestampToLocal(borrowRecordNode.BorrowTime);

    // bug
    strftime(borrowTimeBuff, sizeof(borrowTimeBuff), "%Y-%m-%d %H:%M:%S", &structBorrowTime);

/*    structReturnTime = TimestampToLocal(borrowRecordNode.ReturnTime);
    strftime(returnTimeBuff, sizeof(returnTimeBuff), "%Y-%m-%d %H:%M:%S", &structReturnTime);*/

    printf("\t书籍ID : %d\r\n\t书名 : %s\r\n\t借阅时间 : %s %s\r\n\t归还时间 : %s\r\n\t",
           borrowRecordNode.BookID, borrowRecordNode.BookName,
           borrowTimeBuff, tzname[0], returnTimeBuff);
}

// 输出读者节点
void PrintBorrowerNode(PtrBorrower PtrBorrowerNode)
{
    printf("读者ID : %d\r\n姓名 : %s\r\n已借阅数量 : %d\r\n",
           PtrBorrowerNode->ID, PtrBorrowerNode->Name, PtrBorrowerNode->CurrentBorrowingNum);

    for(int i = 0; i < PtrBorrowerNode->CurrentBorrowingNum; i++)
    {
        PrintfBorrowRecordNode(PtrBorrowerNode->BorrowingRecord[i]);
    }
}

// 输出读者链表信息，顺序输出
void PrintBorrowerList(PtrBorrower PtrBorrowerHead)
{
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    if(IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("读者链表为空");
    }
    else
    {
        while (!IsBorrowerNodeEmpty(PtrBorrowerCur))
        {
            PrintBorrowerNode(PtrBorrowerCur);
            PtrBorrowerCur = PtrBorrowerCur->Next;
        }
    }
}

// 查询图书
void QueryBook(PtrBook PtrBookHead)
{
    int queryMode = -1;
    int bookID;
    char bookName[BOOK_NAME_LEN];

    printf("--------------------图书查询--------------------\r\n"
           "输入 1 按照书号进行查找(具有唯一性)\r\n输入 2 按照书名进行查找(不具有唯一性)\r\n");
    scanf("%d", &queryMode);

    switch (queryMode)
    {
        case 1:
            printf("输入书籍ID : ");
            scanf("%d", &bookID);
            PtrBook PtrBookNode = FindBookByID(PtrBookHead, bookID);
            PtrBookNode
            ? PrintBookNode(PtrBookNode)
            : printf("目前该图书馆还没有存入该图书，请等待一段时间再进行查找");
            break;
        case 2:
            printf(("输入书名 : "));
            scanf("%s", bookName);
            PtrBook PtrBookList = FindBookByName(PtrBookHead, bookName);
            PtrBookList->Next
            ? PrintBookList(PtrBookList)
            : printf("目前该图书馆还没有存入该图书，请等待一段时间再进行查找");
            ReleaseBookList(PtrBookList);
            break;
        default :
            printf("请输入正确的操作序号！\n");
            break;
    }
}

// 查询读者
void QueryBorrower(PtrBorrower PtrBorrowerHead)
{
    int queryMode = -1;
    int borrowerID;
    char borrowerName[READER_NAME_LEN];

    printf("--------------------读者查询--------------------\r\n"
           "输入 1 按照读者ID进行查找(具有唯一性)\r\n输入 2 按照读者姓名进行查找(不具有唯一性)\r\n");
    scanf("%d", &queryMode);

    switch (queryMode)
    {
        case 1:
            printf("输入读者ID : ");
            scanf("%d", &borrowerID);
            PtrBorrower PtrBorrowerNode = FindBorrowerByID(PtrBorrowerHead, borrowerID);
            PtrBorrowerNode
            ? PrintBorrowerNode(PtrBorrowerNode)
            : printf("目前该读者未录入，请等待一段时间再进行查找");
            break;
        case 2:
            printf(("输入姓名 : "));
            scanf("%s", borrowerName);
            PtrBorrower PtrBorrowerList = FindBorrowerByName(PtrBorrowerHead, borrowerName);
            PtrBorrowerList->Next
            ? PrintBorrowerList(PtrBorrowerList)
            : printf("目前该读者未录入，请等待一段时间再进行查找");
            ReleaseBorrowerList(PtrBorrowerList);
            break;
        default :
            printf("请输入正确的操作序号！\n");
            break;
    }
}

// 检查书籍是否可以借阅
bool IsBookAllowBorrow(PtrBook PtrBookNode)
{
    bool isAllow = false;

    if(IsBookNodeEmpty(PtrBookNode))
    {
        printf("该书籍未录入");
    }
    else if(PtrBookNode->BorrowedNum == PtrBookNode->Inventory)
    {
        printf("该图书已借完");
    }
    else
    {
        isAllow = true;
    }
    return isAllow;
}

// 检查读者是否可以借阅
bool IsBorrowerAllowBorrow(PtrBorrower PtrBorrowerNode)
{
    bool isAllow = false;

    if(IsBorrowerNodeEmpty(PtrBorrowerNode))
    {
        printf("该读者未录入");
    }
    else if(!PtrBorrowerNode->CanBorrow)
    {
        printf("该读者借阅数量已满");
    }
    else
    {
        isAllow = true;
    }
    return isAllow;
}

// 检查是否允许借阅
// 判断借阅书籍是否还有库存，以及读者是否已借阅该图书
bool IsAllowBorrow(PtrBook PtrBookNode, PtrBorrower PtrBorrowerNode)
{
    bool isAllow = true;

    if(IsBookAllowBorrow(PtrBookNode))
    {
        for(int i = 0; i < PtrBorrowerNode->CurrentBorrowingNum; i++)
        {
            if(PtrBookNode->ID == PtrBorrowerNode->BorrowingRecord[i].BookID)
            {
                printf("这本书您已借阅");
                isAllow = false;
                break;
            }
        }
    }
    else
    {
        isAllow = false;
    }

    return isAllow;
}

// 完成书籍借阅，并更新读者和书籍对应节点的值
void AssignedBookToBorrower(PtrBook PtrBookNode, PtrBorrower PtrBorrowerNode)
{
    if(IsAllowBorrow(PtrBookNode, PtrBorrowerNode))
    {
        // 书籍借阅量+1
        PtrBookNode->BorrowedNum++;
        // 读者借阅记录索引
        int index = PtrBorrowerNode->CurrentBorrowingNum;
        PtrBorrowerNode->BorrowingRecord[index].BookID = PtrBookNode->ID;
        strcpy(PtrBorrowerNode->BorrowingRecord[index].BookName , PtrBookNode->Name);
        PtrBorrowerNode->BorrowingRecord[index].BorrowTime = GetTimestamp();
        PtrBorrowerNode->BorrowingRecord[index].ReturnTime = 0;
        // 读者借阅量+1
        PtrBorrowerNode->CurrentBorrowingNum++;
        PtrBorrowerNode->CanBorrow = PtrBorrowerNode->CurrentBorrowingNum < MAX_BORROW_NUM
                                     ? true : false;
        printf("借阅成功，祝您阅读愉快！\r\n");
    }
}

// 借阅书籍
void BorrowBook(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead)
{
    char bookName[BOOK_NAME_LEN];
    int bookId, borrowerID;
    int mode;

    printf("----------------------欢迎来此图书馆借阅，祝您阅读愉快呀！---------------------\r\n请输入读者编号 : \r\n");
    scanf("%d",&borrowerID);
    PtrBorrower PtrBorrowerCur = FindBorrowerByID(PtrBorrowerHead, borrowerID);
    PtrBook PtrBookCur;

    if(IsBorrowerAllowBorrow(PtrBorrowerCur))
    {
        printf("输入 1 会根据书号帮您借阅此书，输入 2 会根据书名帮您借阅此书(书名可能会出现重复)\r\n");
        scanf("%d",&mode);

        switch (mode)
        {
            case 1:
                printf("请输入书号：\r\n");
                scanf("%d",&bookId);
                PtrBookCur = FindBookByID(PtrBookHead, bookId);
                AssignedBookToBorrower(PtrBookCur, PtrBorrowerCur);
                break;
            case 2:
                printf("请输入书名：\r\n");
                scanf("%s",bookName);
                PtrBook PtrBookList = FindBookByName(PtrBookHead, bookName);
                if(!IsBookNodeEmpty(PtrBookList->Next))
                {
                    // 根据书名找到书籍的索引
                    int index;
                    printf("请根据序号选择要借阅的书籍 : \r\n");
                    PrintBookList(PtrBookList);
                    scanf("%d", &index);
                    PtrBookCur = FindBookByIndex(PtrBookList, index);
                    AssignedBookToBorrower(FindBookByID(PtrBookHead, PtrBookCur->ID), PtrBorrowerCur);
                }
                else
                {
                    printf("目前该图书馆还没有存入该图书，请等待一段时间再进行查找");
                }
                ReleaseBookList(PtrBookList);
                break;
            default:
                printf("请输入正确的序号！\r\n");
                break;
        }
    }
}

// 归还书籍
void ReturnBook(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead)
{
    char bookName[BOOK_NAME_LEN];
    int bookId, borrowerID;
    int mode;

    printf("----------------------欢迎来此图书馆，祝您阅读愉快呀！---------------------\r\n请输入读者编号 : \r\n");
    scanf("%d",&borrowerID);
    PtrBorrower PtrBorrowerCur = FindBorrowerByID(PtrBorrowerHead, borrowerID);
    PtrBook PtrBookCur;

    if(!IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("输入 1 会根据书号归还图书，输入 2 会根据书名归还书\r\n");
        scanf("%d",&mode);
        switch (mode)
        {
            case 1:
                printf("请输入书号：\r\n");
                scanf("%d",&bookId);
                int CurrentBorrowingNum = PtrBorrowerCur->CurrentBorrowingNum;
                for(int i = 0; i < CurrentBorrowingNum; i++)
                {
                    if(PtrBorrowerCur->BorrowingRecord[i].BookID == bookId)
                    {
                        for(int j = i; j < CurrentBorrowingNum - 1; j++)
                        {
                            PtrBorrowerCur->BorrowingRecord[j] = PtrBorrowerCur->BorrowingRecord[j + 1];
                        }
                        PtrBorrowerCur->CurrentBorrowingNum--;
                        PtrBorrowerCur->CanBorrow = true;
                        PtrBorrowerCur->BorrowingRecord[i].ReturnTime = GetTimestamp();

                        FindBookByID(PtrBookHead, bookId)->BorrowedNum--;
/*                        PtrBorrowerCur->CanBorrow = PtrBorrowerCur->CurrentBorrowingNum < MAX_BORROW_NUM
                                ? true : false;*/
                        printf("还书成功，欢迎下次借阅！\r\n");
                        break;
                    }
                }
                printf("您没有借阅此书，请检查书号是否输入正确或者请借阅此书后再进行还书哦！\r\n");
                break;
            case 2:
                printf("请输入书名：\r\n");
                scanf("%s",bookName);
                CurrentBorrowingNum = PtrBorrowerCur->CurrentBorrowingNum;
                for(int i = 0; i < CurrentBorrowingNum; i++)
                {
                    if(strcmp(PtrBorrowerCur->BorrowingRecord[i].BookName, bookName) == 0)
                    {
                        for(int j = i; j < CurrentBorrowingNum - 1; j++)
                        {
                            PtrBorrowerCur->BorrowingRecord[j] = PtrBorrowerCur->BorrowingRecord[j + 1];
                        }
                        PtrBorrowerCur->CurrentBorrowingNum--;
                        PtrBorrowerCur->CanBorrow = true;
                        bookId = PtrBorrowerCur->BorrowingRecord[i].BookID;
                        FindBookByID(PtrBookHead, bookId)->BorrowedNum--;
/*                        PtrBorrowerCur->CanBorrow = PtrBorrowerCur->CurrentBorrowingNum < MAX_BORROW_NUM
                                ? true : false;*/
                        printf("还书成功，欢迎下次借阅！\r\n");
                        break;
                    }
                }
                printf("您没有借阅此书，请检查书号是否输入正确或者请借阅此书后再进行还书哦！\r\n");
                break;
            default:
                break;
        }
    }
    else
    {
        printf("该读者未录入");
    }
}

// 图书管理
void BookManagement(PtrBook PtrBookHead)
{
    int mode = -1;
    bool isQuit = false;

    while (!isQuit)
    {
        printf("\r\n----------------------------欢迎使用图书管理系统----------------------------\r\n");
        printf("1 功能为录入图书！\r\n");
        printf("2 功能为查找图书！\r\n");
        printf("3 功能返回上层菜单！\r\n");
        printf("----------------------------------------------------------------------------\r\n");
        printf("请输入相应的功能序号：\r\n");
        scanf("%d", &mode);
        rewind(stdin);

        switch (mode)
        {
            case 1:
                IsBookNodeEmpty(PtrBookHead->Next)
                ? CreateBookList(PtrBookHead)
                : InsertBook(PtrBookHead);
                break;
            case 2:
                QueryBook(PtrBookHead);
                break;
            case 3:
                isQuit = true;
                break;
            default:
                break;
        }
    }
}

// 读者管理
void BorrowerManagement(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead)
{
    int mode = -1;
    bool isQuit = false;

    while (!isQuit)
    {
        printf("\r\n----------------------------欢迎使用读者管理系统----------------------------\r\n");
        printf("1 功能为录入读者！\r\n");
        printf("2 功能为借阅图书！\r\n");
        printf("3 功能为还书！\r\n");
        printf("4 功能为查找读者！\r\n");
        printf("5 功能返回上层菜单！\r\n");
        printf("----------------------------------------------------------------------------\r\n");
        printf("请输入相应的功能序号：\r\n");
        scanf("%d", &mode);
        rewind(stdin);

        switch (mode)
        {
            case 1:
                IsBorrowerNodeEmpty(PtrBorrowerHead->Next)
                ? CreateBorrowerList(PtrBorrowerHead)
                : InsertBorrower(PtrBorrowerHead);
                break;
            case 2:
                BorrowBook(PtrBookHead, PtrBorrowerHead);
                break;
            case 3:
                ReturnBook(PtrBookHead, PtrBorrowerHead);
                break;
            case 4:
                QueryBorrower(PtrBorrowerHead);
                break;
            case 5:
                isQuit = true;
                break;
            default:
                break;
        }
    }
}

// 释放图书链表内存
//  p->next = NULL和p!=NULL的区别
// while(p->next ！= NULL) 循环结束时，此时p的位置是尾节点的位置，但如果用于输出函数的判断条件，则尾节点的数据不会输出。
// while(p!=NULL) 循环结束时， 此时p指向的位置为尾节点的下一个节点，因为没有申请内存空间，所以是一个未知的区域。
void ReleaseBookList(PtrBook PtrBookHead)
{
    PtrBook PtrBookCur, PtrBookTemp;

    PtrBookCur = PtrBookHead;
    while (!IsBookNodeEmpty(PtrBookCur))
    {
        PtrBookTemp = PtrBookCur;
        PtrBookCur = PtrBookCur->Next;
        free(PtrBookTemp);
    }
}

// 释放读者链表内存
void ReleaseBorrowerList(PtrBorrower PtrBorrowerHead)
{
    PtrBorrower PtrBorrowerCur, PtrBorrowerTemp;

    PtrBorrowerCur = PtrBorrowerHead;
    while (!IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        PtrBorrowerTemp = PtrBorrowerCur;
        PtrBorrowerCur = PtrBorrowerCur->Next;
        free(PtrBorrowerTemp);
    }
}

int main()
{
    // 书籍链表
    // 头指针，注意与头节点的区别
    // 头指针指向头节点，若该链表无头节点则指向第一个数据节点
    // 头指针不保存数据，可以保存链表长度
    // 头指针永远不为空
    PtrBook PtrBookHead = (PtrBook) malloc(sizeof(Book));
    if (IsBookNodeEmpty(PtrBookHead))
    {
        printf("创建图书链表分配内存失败");
        exit(0);
    }
    PtrBookHead->Length = 0;
    PtrBookHead->IsHead = true;
    PtrBookHead->Next = NULL;
    PtrBook PtrBookTail = PtrBookHead;
    // 借阅者链表
    PtrBorrower PtrBorrowerHead = (PtrBorrower) malloc(sizeof(Borrower));
    if (IsBorrowerNodeEmpty(PtrBorrowerHead))
    {
        printf("创建读者链表分配内存失败");
        exit(0);
    }
    PtrBorrowerHead->Length = 0;
    PtrBorrowerHead->IsHead = true;
    PtrBorrowerHead->Next = NULL;
    PtrBorrower BorrowerTail = PtrBorrowerHead;

    int mode;
    bool isQuit = false;

    while(!isQuit)
    {
        printf("-----------------------------------------欢迎使用xx大学的图书管理系统-----------------------------------------\n\n");
        printf("\t输入 1 会进入图书管理系统！\n\n");
        printf("\t输入 2 会进入借阅者管理系统！\n\n");
        printf("\t输入 3 会退出该系统！\n\n");
        printf("--------------------------------------------------------------------------------------------------------------\n\n");
        printf("\n\n");

        printf("请输入相应的序号：\n");
        scanf("%d",&mode);
        rewind(stdin);

        switch(mode)
        {
            case 1:
                BookManagement(PtrBookHead);
                break;
            case 2:
                BorrowerManagement(PtrBookHead, PtrBorrowerHead);
                break;
            case 3:
                isQuit = true;
                break;
            default :
                printf("请输入正确的功能序号！\n\n");
                break;
        }
    }

    ReleaseBookList(PtrBookHead);
    ReleaseBorrowerList(PtrBorrowerHead);
    system("pause");

    return(0);
}


