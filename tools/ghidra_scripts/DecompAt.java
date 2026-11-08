// Decompile one address and dump its disassembly/callers.
// Usage: -postScript DecompAt.java <hexaddr> <outfile>
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class DecompAt extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        long va = Long.parseLong(args[0].replace("0x", ""), 16);
        String outName = args.length > 1 ? args[1] : "decomp_at.c";
        Address addr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(va);
        Function f = getFunctionAt(addr);
        if (f == null) {
            try { disassemble(addr); } catch (Exception e) { }
            f = createFunction(addr, null);
        }
        PrintWriter pw = new PrintWriter(new BufferedWriter(new FileWriter(outName)));
        pw.println("// address " + addr + " function " + (f == null ? "null" : f.getName()));
        if (f != null) {
            pw.println("// body " + f.getBody().getMinAddress() + " .. " + f.getBody().getMaxAddress());
            pw.println("// callers:");
            for (Function c : f.getCallingFunctions(monitor)) pw.println("//   " + c.getName() + " @ " + c.getEntryPoint());
            DecompInterface ifc = new DecompInterface();
            ifc.toggleCCode(true);
            ifc.toggleSyntaxTree(true);
            ifc.openProgram(currentProgram);
            DecompileResults res = ifc.decompileFunction(f, 120, monitor);
            if (res != null && res.decompileCompleted()) {
                pw.println(res.getDecompiledFunction().getC());
            } else {
                pw.println("// decompile failed: " + (res == null ? "null" : res.getErrorMessage()));
            }
            ifc.dispose();
        }
        // disassembly of a window around the address
        pw.println("\n// ---- disassembly ----");
        Listing lst = currentProgram.getListing();
        Address a = addr.subtract(0x40);
        for (int i = 0; i < 400; i++) {
            Instruction ins = lst.getInstructionAt(a);
            if (ins != null) pw.println(ins.getAddress() + "  " + ins.toString());
            a = a.add(1);
            if (a == null) break;
        }
        pw.close();
        println("DecompAt: wrote " + outName);
    }
}
