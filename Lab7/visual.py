import matplotlib.pyplot as plt
import pandas as pd

# Data
cache_sizes = [10, 50, 100, 250, 500]
fifo_faults = [9916, 9515, 9018, 7534, 5130]
lru_faults = [9915, 9510, 9029, 7532, 5206]
sc_faults = [9915, 9513, 9033, 7528, 5169]

# Table
df = pd.DataFrame({
    "Cache Size": cache_sizes,
    "FIFO Page Faults": fifo_faults,
    "LRU Page Faults": lru_faults,
    "Second Chance Page Faults": sc_faults
})
print(df.to_string(index=False))

# Graph
plt.figure()
plt.plot(cache_sizes, fifo_faults, marker='o', label='FIFO')
plt.plot(cache_sizes, lru_faults, marker='o', label='LRU')
plt.plot(cache_sizes, sc_faults, marker='o', label='Second Chance')
plt.xlabel("Cache Size")
plt.ylabel("Page Faults")
plt.title("Page Faults vs Cache Size")
plt.legend()
plt.grid(True)
plt.show()
