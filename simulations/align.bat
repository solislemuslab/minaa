@echo off 

set nodes=10 30 50 100 250 500
(for %%n in (%nodes%) do (
    set per=0.05 0.1 0.9
    (for %%p in (%per%) do ( 
        for /l %%i in (1, 1, 30) do (
            "..\minaa.exe" networks\G-%%n-%%p-%%i.csv networks\H-%%n-%%p-%%i.csv
        )
    ))
))
