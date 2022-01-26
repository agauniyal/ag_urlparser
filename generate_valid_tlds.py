import idna

valid_tlds = set()

with open("tld_list.dat", "rt") as f1:
    for line in f1:
        if not line.startswith("//"):
            tld = line.strip()
            if tld.startswith("*."):
                tld = tld[2:]
            if tld.startswith("!"):
                tld = tld[1:]
            if len(tld) != 0:
                valid_tlds.add(tld)
                valid_tlds.add(idna.encode(tld).decode('utf-8'))

valid_tlds = list(valid_tlds)
valid_tlds.sort(key=lambda x: len(x), reverse=True)
valid_tlds = ['"' + tld + '",\n' for tld in valid_tlds]

with open("tld_list.txt", "w") as f2:
    f2.write("".join(valid_tlds))
