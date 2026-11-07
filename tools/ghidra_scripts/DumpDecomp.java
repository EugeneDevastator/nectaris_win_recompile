// Dump every function's decompiled C to one file.
// Usage (headless):
//   analyzeHeadless <proj> <name> -process Nec.exe -noanalysis \
//     -scriptPath <dir> -postScript DumpDecomp.java <outfile>
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class DumpDecomp extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        File out = new File(args.length > 0 ? args[0] : "decomp_all.c");
        DecompInterface ifc = new DecompInterface();
        ifc.toggleCCode(true);
        ifc.toggleSyntaxTree(true);
        ifc.openProgram(currentProgram);

        PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(out)));
        FunctionManager fm = currentProgram.getFunctionManager();
        int n = 0, ok = 0;
        for (Function f : fm.getFunctions(true)) {
            if (monitor.isCancelled()) break;
            n++;
            pw.println("/* ================= " + f.getName() + " @ "
                       + f.getEntryPoint() + " ================= */");
            DecompileResults res = ifc.decompileFunction(f, 90, monitor);
            if (res != null && res.decompileCompleted()) {
                pw.println(res.getDecompiledFunction().getC());
                ok++;
            } else {
                pw.println("// decompile failed: "
                           + (res == null ? "null" : res.getErrorMessage()));
            }
            pw.println();
        }
        pw.close();
        ifc.dispose();
        println("DumpDecomp: wrote " + ok + "/" + n + " functions to " + out);
    }
}
