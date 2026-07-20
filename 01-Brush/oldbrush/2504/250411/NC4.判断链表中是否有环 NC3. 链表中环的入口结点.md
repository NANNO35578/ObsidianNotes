[判断链表中是否有环](https://www.nowcoder.com/profile/386104436/codeBookDetail?submissionId=457291967)
- 快慢指针
- 对于链表找环路的问题，有一个通用的解法——快慢指针（Floyd 判圈法）。给定两个指针，分别命名为slow 和fast，起始位置在链表的开头。每次fast 前进两步，slow 前进一步。如果fast可以走到尽头，那么说明没有环路；如果fast 可以无限走下去，那么说明一定有环路，且一定存在一个时刻slow 和fast 相遇。当slow 和fast 第一次相遇时，我们将fast 重新移动到链表开头，并让slow 和fast 每次都前进一步。当slow 和fast 第二次相遇时，相遇的节点即为环路的开始点
```cpp
    ListNode* EntryNodeOfLoop(ListNode* pHead) {
        if (pHead->next == pHead) return pHead;
        if (pHead== nullptr) return nullptr;
 
        ListNode* pFast = pHead;
        ListNode* pSlow = pHead;
        while (pFast && pSlow) {
            if (pFast->next == nullptr || pFast->next->next == nullptr)
                return nullptr;
            else
                pFast = pFast->next->next;
 
            pSlow = pSlow->next;
            if (pSlow == pFast) break;
        }
 
        pFast = pHead;
        while (pFast != pSlow) {
            pFast = pFast->next;
            pSlow = pSlow->next;
        }
        return pFast;
    }
```
