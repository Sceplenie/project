import json
import argparse
import matplotlib.pyplot as plt

def modulator_from_mode(mode: str) -> str:
  match mode:
    case '1':
      return 'QPSK'
    case '2':
      return '16QAM'
    case '3':
      return '64QAM'
  return 'Unsupported Modulation Mode'


def plot_chart(data):
  plt.figure(figsize=(10, 6))

  for mode, sigma_map in sorted(data.items(), key=lambda x: int(x[0])):
    sigmas = sorted(sigma_map.keys())

    bers = []
    for sigma in sigmas:
      bers.append(sigma_map[sigma])

    plt.plot(
      sigmas,
      bers,
      marker="o",
      label=modulator_from_mode(mode)
    )
    
  plt.xlabel("Sigma")
  plt.ylabel("BER")
  plt.title("BER vs Sigma for Modulation Modes")
  plt.yscale("log")

  plt.grid(True, which="both", linestyle="--", alpha=0.5)
  plt.legend()
  plt.tight_layout()

  plt.show()


def parse_args():
  parser = argparse.ArgumentParser(
    description="Plot BER vs Sigma for modulation modes"
  )

  parser.add_argument(
    "--json",
    required=True,
    help="Path to JSON containing data for chart."
  )

  return parser.parse_args()


def main():
  args = parse_args()

  with open(args.json, "r", encoding="utf-8") as f:
    data = json.load(f)
  
  if not data:
    print('Invalid path for --json argument.')
  else:
    plot_chart(data)


if __name__ == '__main__':
    main()
