//给定一个链表，判断链表中是否有环。
//如果链表中有某个节点，可以通过连续跟踪 next 指针再次到达，则链表中存在环。 为了表示给定链表中的环
//我们使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。 如果 pos 是 - 1，则在该链表中没有环。
//注意：pos 不作为参数进行传递，仅仅是为了标识链表的实际情况。
//如果链表中存在环，则返回 true 。 否则，返回 false 。

//题目是可以使用很简单的方法求解出来的，但是事实上这个题目并不是想象的代码量少，思路简单
//增加：请证明快指针是如何追上慢指针的？
//如果慢指针一次走一步，快指针一次为什么走两步？走三步或者四步不行吗？走n步不行吗？
//假设slow每次走一步，fast每次走3步，那么当进入环之后他们之间的差值是x，每走一步他们之间的距离缩小2，也就是剩下x-2的距离，当x为奇数的时候刚刚好
//就会岔开0这个时候变为-1，就永远不会相遇，当偶数的时候是有可能相遇的
//fast每次走4步，中间相差3步可是有可能遇不到的
//所以这个题最好的方法就是fast走2步，那么进入环之后他们相差1步，那么总有一次fast会追上slow，一定不会错过
bool hasCycle(struct ListNode *head) {
	struct ListNode* fast = head, *slow = head;
	//因为你并不能确定这个链表一定是带环的，所以循环的条件还是要按照正常的思路来写的
	while (fast && fast->next)
	{
		//慢指针走的慢，快指针一次走两步走的快，如果有环，那么总会达到快指针追上了慢指针
		slow = slow->next;
		fast = fast->next->next;
		//他们都进入了环里，快指针追上了慢指针
		if (slow == fast)
			return true;
	}
	return false;

}


//给定一个链表，返回链表开始入环的第一个节点。 如果链表无环，则返回 null。
//一共两种方法
//①写起来麻烦，但是理解很简单：一个指针走一步，一个指针走两步，那么总会在环里相遇，那么那个点就是相遇点，然后我让这个环从这个相交点断开，一个是最开始的头，另一个你设置一个新的头（断开点的下一个位置），那么你让两个链表走，就会发现有相交的点，就转换为了原来做过题的类型了（链表的相交）
//②写起来简单，但是需要公式去证明，并且理解起来很复杂（知其然知其所以然）
//慢指针不可能在环里走的距离超过环的周长，所以慢指针走的长度一定为L+X的距离，快指针走的距离是L+X+C（其中L是进入链表前的距离，X表示从进入环开始到相遇之间的差值，C表示这个环的周长）  2*（L+X） = L+X+C（快指针走的速度是慢指针的两倍，那么走的距离也应该是慢指针的两倍）
//但是：你会发现你上面的推论是错误的，因为当L很长的时候，然而环（C）却很小的时候，你的slow每走一步，如果就可以遍历一遍环，那么当你slow也进入环的时候，你的fast已经在环里面走了很多很多步了，所以上面所推的公式L= C-X是错误的
//真确的公式，快指针：L + N*C + X  然后用这个公式去推最终的结果   2*（L+X）= L + N*C + X   结论L=NC-X，你会发现上面错误的答案只不过是正确答案的一种情况而已
//后面的步骤和上面第一种的解题思路一样，定义两个头，然后他们就会在L处相遇   
struct ListNode *detectCycle(struct ListNode *head) {
	struct ListNode* slow = head, *fast = head;
	while (fast && fast->next)
	{
		//这一步是在最后进行测试用例不过的时候返回来判断发现的一个问题，就是你一开始的快慢指针就是相等的，那么你已进入这个循环的时候提酒直接break了
		slow = slow->next;
		fast = fast->next->next;
		//所以这里选择先进行走一步，然后判断，不然就会直接跳出来了
		if (slow == fast)
			break;
	}
	//上面的循环跳出来有两种可能，第一如果链表是带环的，那么一定以break跳出来，那个时候就找到了快慢指针的相遇点，第二种就是都遍历完了，fast都走到NULL的时候依旧没有找到两个相遇的点，说明这个链表是不带环的。
	//这段代码表示这个链表不带回环，当为奇数个的时候fast为NULL，偶数个的时候fast->next为NULL，但是不管哪种情况出现，都可以表示这是一个没有回环的链表
	if (fast == NULL || fast->next == NULL)
		return NULL;
	struct ListNode * meet = fast;
	while (head != meet)
	{
		head = head->next;
		meet = meet->next;
	}
	return meet;
}
//明天吧第一种方法用自己的方法进行实现


//现有一链表的头指针 ListNode* pHead，给一定值x，编写一段代码将所有小于x的结点排在其余结点之前，
//且不能改变原来的数据顺序，返回重新排列后的链表的头指针。

//但是你会发现提交代码之后他会给你报一个“内存超限”的错误警告，代码不能通过，这之中情况一般下就是你的循环部分
//写成了一个死循环，导致代码相当于形成了一个环
//对于原链表 1 9 3 5 6 7
// 1 3
// 9 5 6 7 这样你写的代码是没有毛病的，因为最后7的后面是NULL的（必须保证链表的最后一个所指向的下一个为空）
//但是
//对于原链表 1 9 3 5 6 7 2的时候就出现问题了
//因为cur往下来遍历，但是你的7最后又指向的2这个节点，就有问题了
typedef struct ListNode ListNode;
class Partition {
public:
	ListNode* partition(ListNode* pHead, int x) {
		//创建带哨兵位的头结点，里面并不存储有效的内容
		ListNode* lessHead, *lessTail;
		ListNode* greaterHead, *greaterTail;
		//你会到最后发现lessTail和greaterTail最后就会走到有效的结点上面
		lessHead = lessTail = (ListNode*)malloc(sizeof(ListNode));
		greaterHead = greaterTail = (ListNode*)malloc(sizeof(ListNode));
		//这里记得要对所开辟的结点进行初始化，如果你不进行初始化，那么他的下一个值是一个随机值
		lessHead->next = greaterHead->next = NULL;
		ListNode* cur = pHead;
		while (cur)
		{
			if (cur->val < x)
			{
				lessTail->next = cur;
				lessTail = lessTail->next;
			}
			else
			{
				greaterTail->next = cur;
				greaterTail = greaterTail->next;
			}
			cur = cur->next;
		}
		lessTail->next = greaterHead->next;
		//解决死循环的办法就是，当下面的那个都遍历完了之后就给他置空
		greaterTail->next = NULL;
		//一定要记住既然你是malloc出来的空间最后一定要进行释放，但是你现在里面释放掉了之后，我能怎么能找到
		//这个新的排序后的头呢，所以这里要定义一个变量来保存它的头
		ListNode* list = lessHead->next;
		free(lessHead);
		free(greaterHead);
		return list;
	}
};


//编写一个程序，找到两个单链表相交的起始节点。
//链表的相交问题
//这里相交可不能用值来比较，一定要使用地址来判断，因为只有地址相同的时候，才能指向同一个结点位置
//解题的思路
//先算出两个链表的长度，然后让长的哪一个先走两个之间差值的步数，然后在同时走（那么他们到达相交结点的长度相同）
typedef struct ListNode ListNode;
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB)
{
	//一定要记住不要随便的直接使用题目所给的头，要定义一个变量，然它进行一系列的操作
	//这里你会发现，在有些编译器上面会报错，因为对于C语言来说，他要求你需要把变量定义在域的前面
	//①求出两个链表的各自长度
	ListNode* curA = headA;
	int la = 0;
	while (curA)
	{
		la++;
		curA = curA->next;
	}
	ListNode* curB = headB;
	int lb = 0;
	while (curB)
	{
		lb++;
		curB = curB->next;
	}
	//这里我省略了判断的过程直接假设法，我就直接设长的是curA，短的是curB
	ListNode* longList = headA;
	ListNode* shortList = headB;
	//这里就判断一下我的假设是否正确，如果不正确那就进行修改
	if (lb > la)
	{
		longList = headB;
		shortList = headA;
	}
	//abs是C语言中的库函数，用来用绝对值的
	int gap = abs(la - lb);
	//让长的链表先走之间的差值步数
	while (gap--)
	{
		longList = longList->next;
	}
	//此时之后longList和shortList是一样长的，所以随便判断哪一个都是可以的
	while (longList)
	{
		if (longList == shortList)
		{
			return longList;
		}
		else
		{
			longList = longList->next;
			shortList = shortList->next;
		}
	}
	return NULL;
}