# How to run and change settings

Typical way to run the program:

    ./LabSchrodinger a=0.5 N=512

You can optionaly change parameters using command line arguments, as is shown above by writing 'VAR=VALUE', you can change
<ul>
    <li>highPrecision=True/False (default = True)</li>
    <li>onlyValues=True/False    (default = False)</li>
    <li>N=int                    (default = 256)</li>
    <li>a=double                 (default = 1.0)</li>
    <li>omega=double             (default = 0.05)</li>
    <li>x0=int                   (default = -N/2)</li>
    <li>HO=True/False            (default = True)</li>
</ul>

Programs output:
<ul>
    <li>evalues.txt  (file with eigenvalues in columnt, if HO=True there is second column with deltaE/hw)</li>
    <li>evectors.txt (file with eigenvectors, first columnt first eigenvector etc.)</li>
</ul>
