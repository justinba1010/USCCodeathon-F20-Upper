def _k_sum(nums, l, r, k, target):
    if k == 2:
        i = l
        j = r
        while i < j:
            summ = nums[i] + nums[j]
            if summ == target:
                return [nums[i], nums[j]]
            if summ < target:
                i += 1
            else:
                j -= 1
        return None
    for i in range(l, len(nums) - k + 1):
        temp = _k_sum(nums, i + 1, len(nums) - 1, k - 1, target - nums[i])
        if temp:
            temp.append(nums[i])
            temp.sort()
            return temp
    return None

def k_sum(nums, k, target):
    return _k_sum(nums, 0, len(nums) - 1, k, target)

(n, k, g) = list(map(int, input().split()))
w = list(map(int, input().split()))
w.sort()

answers = k_sum(w, k, g)
if answers:
    answers = list(map(str, answers))
    print(" ".join(answers))
