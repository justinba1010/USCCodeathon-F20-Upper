def two_sum(nums, target):
    i = 0
    j = len(nums)-1
    while i < j:
        summy = nums[i] + nums[j]
        if summy == target:
            return [nums[i], nums[j]]
        if summy < target:
            i += 1
        else:
            j -= 1
    return None

print(two_sum(list(range(100)), 55))
