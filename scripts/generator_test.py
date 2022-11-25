import subprocess
from os.path import abspath
from pathlib import Path
import filecmp

root_path = Path(abspath(__file__)).parent.parent
generator_folder = root_path/"include" / "etl" / "generators"

# Create folder where generator outputs can go for purpose of comparison
test_folder = root_path / "build" / "generator_tmp"
test_folder.mkdir(parents=True, exist_ok= True)

all_ok = True
for generator in generator_folder.iterdir():
    if generator.suffix != ".h":
        continue
    generator_path = str(generator_folder / generator)
    output_name = generator.name[:-12] + ".h"
    output_path = str(test_folder / output_name)

    cog_cmd = [
        "cog",
        "-d",
        "-e",
        f"-o{output_path}",
        "-DHandlers=16",
        "-DNTypes=16",
        "-DIsOneOf=16",
        generator_path
    ]

    subprocess.run(cog_cmd)

    # Compare generator output against actual file output
    actual_path = str(generator_folder.parent / output_name)
    if not filecmp.cmp(actual_path, output_path):
        print(f"Generator for {output_name} does not match file contents")
        all_ok = False

if all_ok:
    print(f"\nAll generator tests passed\n")
    exit(0)
else:
    print(f"\nGenerator tests failed\n")
    exit(1)
