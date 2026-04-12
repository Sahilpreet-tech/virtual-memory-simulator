import streamlit as st
import matplotlib.pyplot as plt
from algorithms import fifo_trace, lru_trace, optimal_trace

st.set_page_config(page_title="OS Virtual Memory Simulator", layout="wide")

# ================= STATE =================
if "page" not in st.session_state:
    st.session_state.page = 0

if "mode" not in st.session_state:
    st.session_state.mode = "PAGING"

if "algo" not in st.session_state:
    st.session_state.algo = "FIFO"

def reset():
    st.session_state.page = 0

# ================= HEADER =================
st.title("🧠 Virtual Memory OS Simulator (PRO VERSION)")

# ================= NAV BAR =================
col1, col2, col3 = st.columns(3)

with col1:
    if st.button("📄 Paging"):
        st.session_state.mode = "PAGING"
        reset()

with col2:
    if st.button("🧱 Segmentation"):
        st.session_state.mode = "SEG"
        reset()

with col3:
    if st.button("📊 Analytics"):
        st.session_state.mode = "ANALYTICS"
        reset()

# ================= INPUT (ONLY FOR PAGING) =================
pages = list(map(int, st.text_input("Page Reference String", "7 0 1 2 0 3 0 4").split()))
frames = st.slider("Frames (Paging only)", 1, 6, 3)
algo = st.selectbox("Algorithm (Paging only)", ["FIFO", "LRU", "Optimal"])

# ================= TRACE =================
if algo == "FIFO":
    trace, faults = fifo_trace(pages, frames)
elif algo == "LRU":
    trace, faults = lru_trace(pages, frames)
else:
    trace, faults = optimal_trace(pages, frames)

# ================= PAGING =================
if st.session_state.mode == "PAGING":

    st.subheader("📄 Paging Simulator")

    colA, colB = st.columns(2)

    with colA:
        if st.button("▶ Next Step"):
            if st.session_state.page < len(trace) - 1:
                st.session_state.page += 1

    with colB:
        if st.button("🔄 Reset"):
            reset()

    page, memory, fault = trace[st.session_state.page]

    # RAM VIEW (CLEAN)
    st.subheader("🧠 RAM Memory")

    cols = st.columns(frames)

    for i in range(frames):
        with cols[i]:
            if i < len(memory):
                if fault:
                    st.error(memory[i])
                else:
                    st.success(memory[i])
            else:
                st.info("Empty")

    # EXPLANATION (SIMPLE)
    if fault:
        st.error(f"Page Fault ❌ → Page {page} loaded from Disk")
    else:
        st.success(f"Page Hit ✅")

    # FINAL STATUS
    st.markdown("## 📊 Result")

    st.metric("Total Page Faults", faults)
    st.metric("Total Page Hits", len(pages) - faults)

    if st.session_state.page == len(trace) - 1:
        st.success("🎉 Paging Simulation Completed!")

# ================= SEGMENTATION =================
elif st.session_state.mode == "SEG":

    st.subheader("🧱 Segmentation Simulator (Memory Layout)")

    seg_input = st.text_input("Segments (name:size)", "code:100,data:200,stack:150")
    limit = st.slider("Memory Size", 200, 1000, 500)

    if st.button("Run Segmentation"):

        segments = seg_input.split(",")
        used = 0

        st.write("### Memory Layout")

        cols = st.columns(len(segments))

        for i, s in enumerate(segments):
            name, size = s.split(":")
            size = int(size)

            with cols[i]:
                if used + size <= limit:
                    used += size
                    st.success(f"{name}\n{size}")
                else:
                    st.error(f"{name}\n{size}")

        st.warning(f"Used Memory: {used}/{limit}")

# ================= ANALYTICS =================
elif st.session_state.mode == "ANALYTICS":

    st.subheader("📊 Algorithm Performance Comparison")

    fifo_faults = fifo_trace(pages, frames)[1]
    lru_faults = lru_trace(pages, frames)[1]
    opt_faults = optimal_trace(pages, frames)[1]

    # BAR CHART
    algos = ["FIFO", "LRU", "Optimal"]
    faults_list = [fifo_faults, lru_faults, opt_faults]

    fig, ax = plt.subplots()
    ax.bar(algos, faults_list)
    ax.set_title("Page Fault Comparison")
    st.pyplot(fig)

    # PIE CHART
    fig2, ax2 = plt.subplots()
    ax2.pie(faults_list, labels=algos, autopct="%1.1f%%")
    ax2.set_title("Fault Distribution")
    st.pyplot(fig2)

    best = algos[faults_list.index(min(faults_list))]

    st.success(f"🏆 Best Algorithm: {best}")