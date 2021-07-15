#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/time.h>


#define MAX_BORROW_NUM 2 //������������
#define BOOK_NAME_LEN 100 //������󳤶�
#define AUTHOR_NAME_LEN 50 //����������󳤶�
#define READER_NAME_LEN 50 //����������󳤶�


enum Status
{
    // �ڴ�������
    MemoryError = 1,
    // ¼��ɹ�
    Succeed,
    // ȡ��¼��
    CancelInput,
    // �ظ�¼��
    RepeatInput,

};

// �鼮��Ϣ
typedef struct bookNode
{
    // �鼮ID
    int ID;
    // �鼮������
    int Length;
    // �鼮���
    int Inventory;
    // �鼮������
    int BorrowedNum;
    // ͷ�ڵ�
    bool IsHead;

    char Name[BOOK_NAME_LEN];
    char Author[AUTHOR_NAME_LEN];

    struct bookNode *Next;
}Book, *PtrBook;

// ���ļ�¼
typedef struct borrowRecord
{
    int BookID;

    char BookName[BOOK_NAME_LEN];

    // Unix ʱ���
    // ����ʱ���
    time_t BorrowTime;
    // �黹ʱ���
    time_t ReturnTime;

}BorrowRecord, *PtrBorrowRecord;

// ����
typedef struct borrowerNode
{
    int ID;
    // ͷ�ڵ�
    bool IsHead;
    // ������������
    int Length;
    // ��������
    char Name[READER_NAME_LEN];
    // ���Ķ����¼
    BorrowRecord BorrowingRecord[MAX_BORROW_NUM];
    // ��ǰ����ͼ����
    int CurrentBorrowingNum;
    // �Ƿ���Խ���
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


// �鼮�ڵ��Ƿ�Ϊ��
bool IsBookNodeEmpty(PtrBook BookNode)
{
    return BookNode ? false : true;
}

// ���߽ڵ��Ƿ�Ϊ��
bool IsBorrowerNodeEmpty(PtrBorrower BorrowerNode)
{
    return BorrowerNode ? false : true;
}

// ���ļ�¼�ڵ��Ƿ�Ϊ��
bool IsBorrowRecordEmpty(PtrBorrowRecord BorrowRecordNode)
{
    return BorrowRecordNode ? false : true;
}

// ��ȡ��ǰʱ���
time_t GetTimestamp()
{
    time_t  timeNow;
    time(&timeNow);
    return timeNow;
}

// ʱ���תUTCʱ��
struct tm TimestampToUTC(time_t timestamp)
{
    return *gmtime(&timestamp);
}

// ʱ���ת����ʱ��
struct tm TimestampToLocal(time_t timestamp)
{
    struct tm structTime;

    //��ȡʱ����Ϣ
    tzset();

    structTime = TimestampToUTC(timestamp);
    structTime.tm_sec -= timezone;
    // ����tm�ṹ
    mktime(&structTime);

    return structTime;
}

// UTCʱ��תʱ���
time_t UTCToTimestamp(struct tm structTime)
{
    return mktime(&structTime);
}

// �״�¼���鼮ʱ�����鼮��������������ͷָ��
enum Status CreateBookList(PtrBook PtrBookHead)
{
    PtrBook PtrBookCur = (PtrBook) malloc(sizeof(Book));

    if(!IsBookNodeEmpty(PtrBookCur))
    {
        printf("\r\n�������ʾ����ͼ��������Ϣ��\r\n¼���ʽ(�Ե����ո�ָ�)����� ���� ���� ���");
        scanf("%d %s %s %d",
              &PtrBookCur->ID, PtrBookCur->Name, PtrBookCur->Author, &PtrBookCur->Inventory);

        PtrBookCur->BorrowedNum = 0;
        PtrBookCur->IsHead = false;
        PtrBookCur->Next = NULL;

        PtrBookHead->Next = PtrBookCur;
        PtrBookHead->Length++;
        printf("ͼ��¼�����");
    }
    else
    {
        printf("¼��ʧ�ܣ������ڴ治��");
        return MemoryError;
    }

    return Succeed;
}

// ����¼���鼮,��δʵ��˫������ѡ��β�巨
// �羭���������ʵ�ֶ��ֲ�ѯ�㷨,�ұ��ֲ�������
enum Status InsertBook(PtrBook PtrBookHead)
{
    PtrBook PtrBookPrev = PtrBookHead;
    // ��ǰ�ڵ㣨����Ϊ��һ�����ݽڵ㣩
    PtrBook PtrBookCur = PtrBookHead->Next;
    PtrBook PtrBookNode = (PtrBook) malloc(sizeof(Book));
    if(!IsBookNodeEmpty(PtrBookNode))
    {
        printf("\r\n!!!�������ʾ����ͼ��������Ϣ��\r\n¼���ʽ(�Ե����ո�ָ�)����� ���� ���� ���");
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
        printf("ͼ��¼�����");
    }
    else
    {
        printf("�����ڴ�ʧ��");
        exit(0);
        // return MemoryError;
    }
    return Succeed;
}

// �����鼮ΨһID�ж��Ƿ���¼��
bool IsBookExists(PtrBook PtrBookHead, int bookID)
{
    return FindBookByID(PtrBookHead, bookID) ? true : false;
}

// ���鼮����ڵ�������
void DeepCopyBookNode(PtrBook PtrBookTargetNode, PtrBook PtrBookSourceNode)
{
    PtrBookTargetNode->ID = PtrBookSourceNode->ID;
    PtrBookTargetNode->BorrowedNum = PtrBookSourceNode->BorrowedNum;
    strcpy(PtrBookTargetNode->Name, PtrBookSourceNode->Name);
    PtrBookTargetNode->Inventory = PtrBookSourceNode->Inventory;
    PtrBookTargetNode->IsHead = PtrBookSourceNode->IsHead;
    strcpy(PtrBookTargetNode->Author, PtrBookSourceNode->Author);
}

// �����鼮ΨһID����ѯ�鼮�����ظýڵ�
// ���ڸ��ݽڵ�ֵ���ҽڵ�λ��
PtrBook FindBookByID(PtrBook PtrBookHead, int bookID)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    if (IsBookNodeEmpty(PtrBookCur))
    {
        printf("δ¼���鼮");
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

// ����������ѯ�鼮�����ش�ͷ�ڵ�Ĳ�ѯ�������
// ���ص���ͷָ��

// ���  ��һ���鼮����ͷָ�� �ڶ��� ����
// ���� ��ѯ�����鼮����
PtrBook FindBookByName(PtrBook PtrBookHead, char *bookName)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    PtrBook PtrBookResultHead = (PtrBook) malloc(sizeof(Book));
    PtrBook PtrBookResultTail, PtrBookResultCur;
    PtrBookResultTail = PtrBookResultHead;

    if(IsBookNodeEmpty(PtrBookResultHead))
    {
        printf("�����ڴ�ʧ��");
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
                    printf("�����ڴ�ʧ��");
                    exit(0);
                }
                else
                {
                    // β�巨
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

// �����鼮�ڵ�λ�ò�ѯ�鼮�����ظýڵ�
PtrBook FindBookByIndex(PtrBook PtrBookHead, int index)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    int currentPosition = 1;
    if (IsBookNodeEmpty(PtrBookCur))
    {
        printf("δ¼���鼮");
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

// ����鼮�ڵ�
void PrintBookNode(PtrBook PtrBookNode)
{
    printf("�鼮ID : %d\r\n�鼮��� : %d\r\n�鼮������ : %d\r\n���� : %s\r\n�鼮���� : %s\r\n",
           PtrBookNode->ID, PtrBookNode->Inventory, PtrBookNode->BorrowedNum,
           PtrBookNode->Name, PtrBookNode->Author);
}

// ����鼮����˳�����
void PrintBookList(PtrBook PtrBookHead)
{
    PtrBook PtrBookCur = PtrBookHead->Next;
    if(IsBookNodeEmpty(PtrBookCur))
    {
        printf("�鼮����Ϊ��");
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

// �״�¼�����ʱ����������������������ͷָ��
enum Status CreateBorrowerList(PtrBorrower PtrBorrowerHead)
{
    PtrBorrower PtrBorrowerCur = (PtrBorrower) malloc(sizeof(Borrower));

    if(!IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("\r\n�������ʾ��������˵������Ϣ��\r\n¼���ʽ(�Ե����ո�ָ�)����� ����");
        scanf("%d %s", &PtrBorrowerCur->ID, PtrBorrowerCur->Name);

        PtrBorrowerCur->CurrentBorrowingNum = 0;
        PtrBorrowerCur->IsHead = false;
        PtrBorrowerCur->CanBorrow = true;
        PtrBorrowerCur->Next = NULL;
        PtrBorrowerHead->Next = PtrBorrowerCur;
        PtrBorrowerHead->Length++;
        printf("������Ϣ¼�����");
    }
    else
    {
        printf("¼��ʧ�ܣ������ڴ治��");
        return MemoryError;
    }

    return Succeed;
}

// ��������
enum Status InsertBorrower(PtrBorrower PtrBorrowerHead)
{
    PtrBorrower PtrBorrowerPrev = PtrBorrowerHead;
    // ��ǰ�ڵ㣨����Ϊ��һ�����ݽڵ㣩
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    PtrBorrower PtrBorrowerNode = (PtrBorrower) malloc(sizeof(Borrower));
    if(!IsBorrowerNodeEmpty(PtrBorrowerNode))
    {
        printf("\r\n�������ʾ������ߵ������Ϣ��\r\n¼���ʽ(�Ե����ո�ָ�)����� ����");
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
        printf("������Ϣ¼�����");
    }
    else
    {
        printf("�����ڴ�ʧ��");
        exit(0);
        // return MemoryError;
    }
    return Succeed;
}

// ���ݶ���ΨһID�ж��Ƿ���¼��
bool IsBorrowerExists(PtrBorrower PtrBorrowerHead, int borrowerID)
{
    return FindBorrowerByID(PtrBorrowerHead, borrowerID) ? true : false;
}

// ���鼮����ڵ�������
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

// ���ݶ���ΨһID����ѯ���߲����ظýڵ�
// ���ڸ��ݽڵ�ֵ���ҽڵ�λ��
PtrBorrower FindBorrowerByID(PtrBorrower PtrBorrowerHead, int borrowerID)
{
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    if (IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("�ö���δ¼��");
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

// �������ֲ�ѯ���ߣ����ش�ͷ�ڵ�Ĳ�ѯ�������
// ���ص���ͷָ��
PtrBorrower FindBorrowerByName(PtrBorrower PtrBorrowerHead, char *borrowerName)
{
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    PtrBorrower PtrBorrowerResultHead = (PtrBorrower) malloc(sizeof(Borrower));
    PtrBorrower PtrBorrowerResultTail, PtrBorrowerResultCur;
    PtrBorrowerResultTail = PtrBorrowerResultHead;

    if(IsBorrowerNodeEmpty(PtrBorrowerResultHead))
    {
        printf("�����ڴ�ʧ��");
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
                    printf("�����ڴ�ʧ��");
                    exit(0);
                }
                else
                {
                    // β�巨
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

// ������ļ�¼
void PrintfBorrowRecordNode(BorrowRecord borrowRecordNode)
{
    char borrowTimeBuff[100], returnTimeBuff[100];
    struct tm structBorrowTime, structReturnTime;

    structBorrowTime = TimestampToLocal(borrowRecordNode.BorrowTime);

    // bug
    strftime(borrowTimeBuff, sizeof(borrowTimeBuff), "%Y-%m-%d %H:%M:%S", &structBorrowTime);

/*    structReturnTime = TimestampToLocal(borrowRecordNode.ReturnTime);
    strftime(returnTimeBuff, sizeof(returnTimeBuff), "%Y-%m-%d %H:%M:%S", &structReturnTime);*/

    printf("\t�鼮ID : %d\r\n\t���� : %s\r\n\t����ʱ�� : %s %s\r\n\t�黹ʱ�� : %s\r\n\t",
           borrowRecordNode.BookID, borrowRecordNode.BookName,
           borrowTimeBuff, tzname[0], returnTimeBuff);
}

// ������߽ڵ�
void PrintBorrowerNode(PtrBorrower PtrBorrowerNode)
{
    printf("����ID : %d\r\n���� : %s\r\n�ѽ������� : %d\r\n",
           PtrBorrowerNode->ID, PtrBorrowerNode->Name, PtrBorrowerNode->CurrentBorrowingNum);

    for(int i = 0; i < PtrBorrowerNode->CurrentBorrowingNum; i++)
    {
        PrintfBorrowRecordNode(PtrBorrowerNode->BorrowingRecord[i]);
    }
}

// �������������Ϣ��˳�����
void PrintBorrowerList(PtrBorrower PtrBorrowerHead)
{
    PtrBorrower PtrBorrowerCur = PtrBorrowerHead->Next;
    if(IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("��������Ϊ��");
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

// ��ѯͼ��
void QueryBook(PtrBook PtrBookHead)
{
    int queryMode = -1;
    int bookID;
    char bookName[BOOK_NAME_LEN];

    printf("--------------------ͼ���ѯ--------------------\r\n"
           "���� 1 ������Ž��в���(����Ψһ��)\r\n���� 2 �����������в���(������Ψһ��)\r\n");
    scanf("%d", &queryMode);

    switch (queryMode)
    {
        case 1:
            printf("�����鼮ID : ");
            scanf("%d", &bookID);
            PtrBook PtrBookNode = FindBookByID(PtrBookHead, bookID);
            PtrBookNode
            ? PrintBookNode(PtrBookNode)
            : printf("Ŀǰ��ͼ��ݻ�û�д����ͼ�飬��ȴ�һ��ʱ���ٽ��в���");
            break;
        case 2:
            printf(("�������� : "));
            scanf("%s", bookName);
            PtrBook PtrBookList = FindBookByName(PtrBookHead, bookName);
            PtrBookList->Next
            ? PrintBookList(PtrBookList)
            : printf("Ŀǰ��ͼ��ݻ�û�д����ͼ�飬��ȴ�һ��ʱ���ٽ��в���");
            ReleaseBookList(PtrBookList);
            break;
        default :
            printf("��������ȷ�Ĳ�����ţ�\n");
            break;
    }
}

// ��ѯ����
void QueryBorrower(PtrBorrower PtrBorrowerHead)
{
    int queryMode = -1;
    int borrowerID;
    char borrowerName[READER_NAME_LEN];

    printf("--------------------���߲�ѯ--------------------\r\n"
           "���� 1 ���ն���ID���в���(����Ψһ��)\r\n���� 2 ���ն����������в���(������Ψһ��)\r\n");
    scanf("%d", &queryMode);

    switch (queryMode)
    {
        case 1:
            printf("�������ID : ");
            scanf("%d", &borrowerID);
            PtrBorrower PtrBorrowerNode = FindBorrowerByID(PtrBorrowerHead, borrowerID);
            PtrBorrowerNode
            ? PrintBorrowerNode(PtrBorrowerNode)
            : printf("Ŀǰ�ö���δ¼�룬��ȴ�һ��ʱ���ٽ��в���");
            break;
        case 2:
            printf(("�������� : "));
            scanf("%s", borrowerName);
            PtrBorrower PtrBorrowerList = FindBorrowerByName(PtrBorrowerHead, borrowerName);
            PtrBorrowerList->Next
            ? PrintBorrowerList(PtrBorrowerList)
            : printf("Ŀǰ�ö���δ¼�룬��ȴ�һ��ʱ���ٽ��в���");
            ReleaseBorrowerList(PtrBorrowerList);
            break;
        default :
            printf("��������ȷ�Ĳ�����ţ�\n");
            break;
    }
}

// ����鼮�Ƿ���Խ���
bool IsBookAllowBorrow(PtrBook PtrBookNode)
{
    bool isAllow = false;

    if(IsBookNodeEmpty(PtrBookNode))
    {
        printf("���鼮δ¼��");
    }
    else if(PtrBookNode->BorrowedNum == PtrBookNode->Inventory)
    {
        printf("��ͼ���ѽ���");
    }
    else
    {
        isAllow = true;
    }
    return isAllow;
}

// �������Ƿ���Խ���
bool IsBorrowerAllowBorrow(PtrBorrower PtrBorrowerNode)
{
    bool isAllow = false;

    if(IsBorrowerNodeEmpty(PtrBorrowerNode))
    {
        printf("�ö���δ¼��");
    }
    else if(!PtrBorrowerNode->CanBorrow)
    {
        printf("�ö��߽�����������");
    }
    else
    {
        isAllow = true;
    }
    return isAllow;
}

// ����Ƿ��������
// �жϽ����鼮�Ƿ��п�棬�Լ������Ƿ��ѽ��ĸ�ͼ��
bool IsAllowBorrow(PtrBook PtrBookNode, PtrBorrower PtrBorrowerNode)
{
    bool isAllow = true;

    if(IsBookAllowBorrow(PtrBookNode))
    {
        for(int i = 0; i < PtrBorrowerNode->CurrentBorrowingNum; i++)
        {
            if(PtrBookNode->ID == PtrBorrowerNode->BorrowingRecord[i].BookID)
            {
                printf("�Ȿ�����ѽ���");
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

// ����鼮���ģ������¶��ߺ��鼮��Ӧ�ڵ��ֵ
void AssignedBookToBorrower(PtrBook PtrBookNode, PtrBorrower PtrBorrowerNode)
{
    if(IsAllowBorrow(PtrBookNode, PtrBorrowerNode))
    {
        // �鼮������+1
        PtrBookNode->BorrowedNum++;
        // ���߽��ļ�¼����
        int index = PtrBorrowerNode->CurrentBorrowingNum;
        PtrBorrowerNode->BorrowingRecord[index].BookID = PtrBookNode->ID;
        strcpy(PtrBorrowerNode->BorrowingRecord[index].BookName , PtrBookNode->Name);
        PtrBorrowerNode->BorrowingRecord[index].BorrowTime = GetTimestamp();
        PtrBorrowerNode->BorrowingRecord[index].ReturnTime = 0;
        // ���߽�����+1
        PtrBorrowerNode->CurrentBorrowingNum++;
        PtrBorrowerNode->CanBorrow = PtrBorrowerNode->CurrentBorrowingNum < MAX_BORROW_NUM
                                     ? true : false;
        printf("���ĳɹ���ף���Ķ���죡\r\n");
    }
}

// �����鼮
void BorrowBook(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead)
{
    char bookName[BOOK_NAME_LEN];
    int bookId, borrowerID;
    int mode;

    printf("----------------------��ӭ����ͼ��ݽ��ģ�ף���Ķ����ѽ��---------------------\r\n��������߱�� : \r\n");
    scanf("%d",&borrowerID);
    PtrBorrower PtrBorrowerCur = FindBorrowerByID(PtrBorrowerHead, borrowerID);
    PtrBook PtrBookCur;

    if(IsBorrowerAllowBorrow(PtrBorrowerCur))
    {
        printf("���� 1 �������Ű������Ĵ��飬���� 2 ����������������Ĵ���(�������ܻ�����ظ�)\r\n");
        scanf("%d",&mode);

        switch (mode)
        {
            case 1:
                printf("��������ţ�\r\n");
                scanf("%d",&bookId);
                PtrBookCur = FindBookByID(PtrBookHead, bookId);
                AssignedBookToBorrower(PtrBookCur, PtrBorrowerCur);
                break;
            case 2:
                printf("������������\r\n");
                scanf("%s",bookName);
                PtrBook PtrBookList = FindBookByName(PtrBookHead, bookName);
                if(!IsBookNodeEmpty(PtrBookList->Next))
                {
                    // ���������ҵ��鼮������
                    int index;
                    printf("��������ѡ��Ҫ���ĵ��鼮 : \r\n");
                    PrintBookList(PtrBookList);
                    scanf("%d", &index);
                    PtrBookCur = FindBookByIndex(PtrBookList, index);
                    AssignedBookToBorrower(FindBookByID(PtrBookHead, PtrBookCur->ID), PtrBorrowerCur);
                }
                else
                {
                    printf("Ŀǰ��ͼ��ݻ�û�д����ͼ�飬��ȴ�һ��ʱ���ٽ��в���");
                }
                ReleaseBookList(PtrBookList);
                break;
            default:
                printf("��������ȷ����ţ�\r\n");
                break;
        }
    }
}

// �黹�鼮
void ReturnBook(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead)
{
    char bookName[BOOK_NAME_LEN];
    int bookId, borrowerID;
    int mode;

    printf("----------------------��ӭ����ͼ��ݣ�ף���Ķ����ѽ��---------------------\r\n��������߱�� : \r\n");
    scanf("%d",&borrowerID);
    PtrBorrower PtrBorrowerCur = FindBorrowerByID(PtrBorrowerHead, borrowerID);
    PtrBook PtrBookCur;

    if(!IsBorrowerNodeEmpty(PtrBorrowerCur))
    {
        printf("���� 1 �������Ź黹ͼ�飬���� 2 ����������黹��\r\n");
        scanf("%d",&mode);
        switch (mode)
        {
            case 1:
                printf("��������ţ�\r\n");
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
                        printf("����ɹ�����ӭ�´ν��ģ�\r\n");
                        break;
                    }
                }
                printf("��û�н��Ĵ��飬��������Ƿ�������ȷ��������Ĵ�����ٽ��л���Ŷ��\r\n");
                break;
            case 2:
                printf("������������\r\n");
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
                        printf("����ɹ�����ӭ�´ν��ģ�\r\n");
                        break;
                    }
                }
                printf("��û�н��Ĵ��飬��������Ƿ�������ȷ��������Ĵ�����ٽ��л���Ŷ��\r\n");
                break;
            default:
                break;
        }
    }
    else
    {
        printf("�ö���δ¼��");
    }
}

// ͼ�����
void BookManagement(PtrBook PtrBookHead)
{
    int mode = -1;
    bool isQuit = false;

    while (!isQuit)
    {
        printf("\r\n----------------------------��ӭʹ��ͼ�����ϵͳ----------------------------\r\n");
        printf("1 ����Ϊ¼��ͼ�飡\r\n");
        printf("2 ����Ϊ����ͼ�飡\r\n");
        printf("3 ���ܷ����ϲ�˵���\r\n");
        printf("----------------------------------------------------------------------------\r\n");
        printf("��������Ӧ�Ĺ�����ţ�\r\n");
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

// ���߹���
void BorrowerManagement(PtrBook PtrBookHead, PtrBorrower PtrBorrowerHead)
{
    int mode = -1;
    bool isQuit = false;

    while (!isQuit)
    {
        printf("\r\n----------------------------��ӭʹ�ö��߹���ϵͳ----------------------------\r\n");
        printf("1 ����Ϊ¼����ߣ�\r\n");
        printf("2 ����Ϊ����ͼ�飡\r\n");
        printf("3 ����Ϊ���飡\r\n");
        printf("4 ����Ϊ���Ҷ��ߣ�\r\n");
        printf("5 ���ܷ����ϲ�˵���\r\n");
        printf("----------------------------------------------------------------------------\r\n");
        printf("��������Ӧ�Ĺ�����ţ�\r\n");
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

// �ͷ�ͼ�������ڴ�
//  p->next = NULL��p!=NULL������
// while(p->next ��= NULL) ѭ������ʱ����ʱp��λ����β�ڵ��λ�ã��������������������ж���������β�ڵ�����ݲ��������
// while(p!=NULL) ѭ������ʱ�� ��ʱpָ���λ��Ϊβ�ڵ����һ���ڵ㣬��Ϊû�������ڴ�ռ䣬������һ��δ֪������
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

// �ͷŶ��������ڴ�
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
    // �鼮����
    // ͷָ�룬ע����ͷ�ڵ������
    // ͷָ��ָ��ͷ�ڵ㣬����������ͷ�ڵ���ָ���һ�����ݽڵ�
    // ͷָ�벻�������ݣ����Ա���������
    // ͷָ����Զ��Ϊ��
    PtrBook PtrBookHead = (PtrBook) malloc(sizeof(Book));
    if (IsBookNodeEmpty(PtrBookHead))
    {
        printf("����ͼ����������ڴ�ʧ��");
        exit(0);
    }
    PtrBookHead->Length = 0;
    PtrBookHead->IsHead = true;
    PtrBookHead->Next = NULL;
    PtrBook PtrBookTail = PtrBookHead;
    // ����������
    PtrBorrower PtrBorrowerHead = (PtrBorrower) malloc(sizeof(Borrower));
    if (IsBorrowerNodeEmpty(PtrBorrowerHead))
    {
        printf("����������������ڴ�ʧ��");
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
        printf("-----------------------------------------��ӭʹ��xx��ѧ��ͼ�����ϵͳ-----------------------------------------\n\n");
        printf("\t���� 1 �����ͼ�����ϵͳ��\n\n");
        printf("\t���� 2 ���������߹���ϵͳ��\n\n");
        printf("\t���� 3 ���˳���ϵͳ��\n\n");
        printf("--------------------------------------------------------------------------------------------------------------\n\n");
        printf("\n\n");

        printf("��������Ӧ����ţ�\n");
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
                printf("��������ȷ�Ĺ�����ţ�\n\n");
                break;
        }
    }

    ReleaseBookList(PtrBookHead);
    ReleaseBorrowerList(PtrBorrowerHead);
    system("pause");

    return(0);
}


