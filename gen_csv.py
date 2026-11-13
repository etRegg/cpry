
#!/usr/bin/env python3
import argparse
import csv
from pathlib import Path

COLUMNAS = ["tipo", "N", "bloque", "tiempo", "rendimiento"]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("prefijo")
    parser.add_argument("--directorio", type=Path, default=Path("."))
    parser.add_argument("--salida", type=Path, default=Path("resultado.csv"))
    args = parser.parse_args()

    filas = []

    archivos = sorted(
        f for f in args.directorio.iterdir()
        if f.is_file()
        and f.name.startswith(args.prefijo)
        and f.suffix.lower() != ".csv"
    )

    for archivo in archivos:
        with archivo.open("r", encoding="utf-8-sig") as entrada:
            for numero, linea in enumerate(entrada, 1):
                linea = linea.strip()
                if not linea:
                    continue

                campos = [x.strip() for x in linea.split(";")]

                if len(campos) != 5:
                    print(f"Registro inválido: {archivo.name}:{numero}")
                    continue

                try:
                    filas.append([
                        campos[0],
                        int(campos[1]),
                        int(campos[2]),
                        float(campos[3]),
                        float(campos[4])
                    ])
                except ValueError:
                    print(f"Datos inválidos: {archivo.name}:{numero}")

    # Orden numérico por tamaño N y bloque
    filas.sort(key=lambda x: (x[1], x[2]))

    with args.salida.open("w", newline="", encoding="utf-8") as salida:
        escritor = csv.writer(salida, delimiter=";")
        escritor.writerow(COLUMNAS)
        escritor.writerows(filas)

    print(f"Archivos procesados: {len(archivos)}")
    print(f"Registros consolidados: {len(filas)}")
    print(f"CSV generado: {args.salida}")


if __name__ == "__main__":
    main()
