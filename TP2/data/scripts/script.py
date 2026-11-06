import random
import os

sizes = [
    100_000,
    200_000,
    400_000,
    600_000,
    800_000,
    1_000_000,
    1_200_000,
    1_400_000,
    1_600_000,
    1_800_000,
    2_000_000,
    4_000_000,
    5_000_000,
    6_000_000,
    7_000_000,
    8_000_000,

]

random.seed(42),
OUTPUT_DIR = "test_data"

def generate_master_dataset(max_n):
    return random.sample(range(1, max_n * 10), max_n)


def generate_unsorted_tables(dataset, sizes):
    tables = {}

    for n in sizes:
        tables[n] = dataset[:n]

    return tables


def generate_sorted_tables(dataset, sizes):
    tables = {}

    for n in sizes:
        tables[n] = sorted(dataset[:n])

    return tables


def save_tables(tables, folder):
    os.makedirs(folder, exist_ok=True)

    for n, table in tables.items():
        filename = os.path.join(folder, f"{n}.txt")

        with open(filename, "w") as file:
            file.write(f"{n}\n")
            file.write(" ".join(map(str, table)))

        print(f"Generated: {filename}")


def main():

    max_n = max(sizes)

    print("Generating master dataset...")
    master_dataset = generate_master_dataset(max_n)

    print("Generating unsorted tables...")
    unsorted_tables = generate_unsorted_tables(
        master_dataset, sizes
    )

    print("Generating sorted tables...")
    sorted_tables = generate_sorted_tables(
        master_dataset, sizes
    )

    print("Saving datasets...")

    save_tables(unsorted_tables, f"{OUTPUT_DIR}/unsorted")
    save_tables(sorted_tables, f"{OUTPUT_DIR}/sorted")

    print("\nAll datasets generated successfully!")


if __name__ == "__main__":
    main()
