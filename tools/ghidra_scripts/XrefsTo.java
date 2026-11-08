// List references (calls/data) to one or more addresses.
// Usage: -postScript XrefsTo.java <hexaddr> [<hexaddr> ...]
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceManager;
import ghidra.program.model.listing.Function;

public class XrefsTo extends GhidraScript {
    public void run() throws Exception {
        ReferenceManager rm = currentProgram.getReferenceManager();
        for (String arg : getScriptArgs()) {
            long va = Long.parseLong(arg.replace("0x", ""), 16);
            Address addr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(va);
            println("=== refs to " + addr + " ===");
            ghidra.program.model.symbol.ReferenceIterator it = rm.getReferencesTo(addr);
            boolean any = false;
            while (it.hasNext()) {
                Reference r = it.next();
                any = true;
                Address from = r.getFromAddress();
                Function f = getFunctionContaining(from);
                println("  " + r.getReferenceType() + " from " + from
                        + (f != null ? "  in " + f.getName() + " @" + f.getEntryPoint() : ""));
            }
            if (!any) println("  (none)");
        }
    }
}
