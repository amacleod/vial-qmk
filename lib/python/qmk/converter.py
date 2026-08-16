"""Functions to convert to and from QMK formats
"""
from collections import OrderedDict


def kle2qmk(kle):
    """Convert a KLE layout to QMK's layout format.
    """
    layout = []

    for row in kle:
        for key in row:
            if key['decal']:
                continue

            qmk_key = OrderedDict(
                label="",
                x=key['column'],
                y=key['row'],
            )

            if key['width'] != 1:
                qmk_key['w'] = key['width']
            if key['height'] != 1:
                qmk_key['h'] = key['height']
            if 'name' in key and key['name']:
                name = key['name']
                if isinstance(name, list):
                    # Some legends use KLE's rich-label array form (mixing
                    # text runs with style-change dicts) instead of a plain
                    # newline-separated string; take the first text run.
                    name = next((part for part in name if isinstance(part, str)), '')
                qmk_key['label'] = name.split('\n', 1)[0]
            else:
                del (qmk_key['label'])

            layout.append(qmk_key)

    return layout
