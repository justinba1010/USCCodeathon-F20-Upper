fouriness n = sum $ map (fourinessDigit n) [0..d]
    where d = truncate (logBase 10 (fromIntegral n))

fourinessDigit n d = topHalf * d10 + extra
    where d10 = 10^d
          (topMidHalf, bottomHalf) = divMod n d10
          (topHalf, digit) = divMod topMidHalf 10
          extra
            | digit < 4           = 0
            | digit == 4 && d > 0 = bottomHalf
            | otherwise           = d10

main = do
    contents <- getContents
    let ms = map (read . last . words) $ lines contents :: [Integer]
    let fs = map fouriness ms
    mapM_ print fs
