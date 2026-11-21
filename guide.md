# RepairGraph — The Intelligence Layer for Repair Businesses

## 1. Project overview

RepairGraph is a console-based C++ prototype that helps laptop or smartphone repair businesses organize and reuse repair knowledge.

Repair technicians often rely on personal experience to connect a device model, customer-reported symptoms, diagnostic tests, possible faults, compatible spare parts, and outcomes from previous repairs. That knowledge is often scattered across notes, spreadsheets, messages, manuals, or individual technicians' memories.

RepairGraph represents this information as a **weighted knowledge graph**. A technician selects a device and enters its symptoms. The program explores related information and returns likely faults, suggests a useful next diagnostic test, and looks up compatible parts. After a repair, the technician records the confirmed fault and outcome so the stored repair knowledge can be improved.

The prototype will focus on one repair category (laptop or smartphone) and use synthetic or technician-reviewed example cases. It is a decision-support prototype, not a replacement for a qualified technician.

## 2. Problem statement

Small and medium repair shops may record what was repaired and what was charged, but often do not have a structured way to connect symptoms, diagnostic tests, faults, parts, and repair history.

This can make it harder to:
- Identify likely causes of a reported problem.
- Choose a useful next diagnostic test.
- Find compatible replacement parts.
- Reuse knowledge from earlier repairs.
- Train less experienced technicians consistently.

RepairGraph aims to connect this information in one searchable model and use graph traversal and ranking algorithms to make relevant repair knowledge easier to find.

## 3. Proposed solution

The program will represent repair knowledge as a graph.

### 3.1 Graph nodes

Each vertex (node) represents an item such as:
- Device or device model.
- Symptom.
- Diagnostic test.
- Fault.
- Spare part.
- Repair outcome.

### 3.2 Graph edges

Edges represent relationships between nodes, for example:
- A device can exhibit a symptom.
- A symptom may indicate a fault.
- A diagnostic test can support or rule out a fault.
- A fault can be fixed by a part or repair action.
- A repair outcome can provide evidence about a relationship.

### 3.3 Edge weights

Edges carry weights based on how strongly two items are related, using recorded cases or reviewed example data. The program must define a consistent meaning for each weight.

For example, if a larger number means a stronger relationship, a shortest-path algorithm cannot simply treat that larger number as a lower cost. We will define a conversion from relationship strength to traversal cost, or choose a ranking method that works directly with strengths.

## 4. Main user features

1. **Diagnose:** The technician selects a device and enters observed symptoms. The system searches the graph and returns ranked possible faults.

2. **Recommend:** For the leading candidate faults, the system suggests a next diagnostic test that may help distinguish between them, along with compatible parts.

3. **Learn:** After the repair, the technician records the confirmed fault and outcome. The system updates the repair history and relevant relationship weights.

4. **Explain:** The program shows why a fault or test was recommended, such as the connected symptom, relevant path, and supporting example cases.

5. **Search:** The technician can search stored symptoms, faults, devices, and parts.

## 5. DSA concepts and their role

The following is a proposed implementation plan based on the project proposal. We should confirm the final set against the instructor's course requirements.

### 5.1 Core concepts

| Concept | Role in RepairGraph |
|---|---|
| Graphs | Model devices, symptoms, tests, faults, parts, and outcomes as connected information. |
| Adjacency lists | Store each graph node's neighboring nodes and edge information. |
| BFS | Explore related nodes level by level, such as nearby relationships from a symptom. |
| DFS | Explore connected diagnostic paths in depth. |
| Weighted graph algorithms | Use edge weights to reason about relationships between symptoms, faults, tests, and parts. |
| Dijkstra's algorithm | Find a minimum-cost path when the graph's edge costs are non-negative and the chosen cost model is appropriate. |
| Heaps | Efficiently select top-ranked fault candidates from a collection of scores. |
| Hash tables | Support quick lookup of nodes by an ID or normalized name. |
| Sorting | Present candidate faults in score order when a full ordered result is needed. |

### 5.2 Optional extensions

| Concept | Possible role |
|---|---|
| Tries | Support prefix-based search for symptom names or terms. |
| String pattern matching | Match words or patterns in free-text symptom descriptions. |
| Trees or BSTs | Provide an additional ordered index if the implementation needs one. |

These extensions should only be added after the core diagnosis workflow works. Avoid adding a data structure just to include its name; each one should solve a real project requirement.

## 6. Suggested implementation architecture

A manageable C++ design can be divided into these logical parts:

- **Data model:** Structures or classes for devices, symptoms, faults, tests, parts, outcomes, and repair cases.
- **Graph:** Stores nodes and weighted edges, preferably using adjacency lists.
- **Search/indexing:** Provides lookup by ID or name, initially using a hash table or a simple lookup structure.
- **Algorithms:** Contains BFS, DFS, Dijkstra (if suitable), and heap-based ranking.
- **Diagnosis logic:** Combines the user's selected symptoms with graph results and creates an explainable ranked list.
- **Data storage:** Loads and saves synthetic records using plain text or CSV files.
- **User interface:** A menu-driven console program for entering symptoms, viewing results, and recording outcomes.

Keep the graph and algorithm code separate from the menu code. This makes each part easier to test and explain.

## 7. Development roadmap

### Phase 1 — Understand and model the data

- Choose one category: laptop or smartphone repair.
- Define the node types and relationship types.
- Create a small hand-written synthetic dataset.
- Decide how node IDs and edge weights will work.

### Phase 2 — Build the graph

- Define a node representation.
- Define an edge representation with a weight.
- Implement an adjacency-list graph.
- Add sample nodes and connections.
- Print the graph to verify its contents.

### Phase 3 — Implement graph traversal

- Implement BFS.
- Implement DFS (iterative or recursive).
- Test both algorithms on small known graphs.
- Use traversal to explore nodes connected to entered symptoms.

### Phase 4 — Rank candidate faults

- Decide what a relationship weight means.
- Define a consistent score or cost model.
- Implement Dijkstra only where its assumptions are satisfied.
- Use a heap to retrieve the strongest candidate scores.
- Explain how each candidate received its score.

### Phase 5 — Add fast search

- Add hash-table lookup for IDs or normalized names.
- Add symptom search.
- If time and course requirements permit, add a trie for prefix search and string matching for free-text descriptions.

### Phase 6 — Build diagnosis and recommendations

- Accept a device and one or more symptoms.
- Find relevant graph nodes.
- Generate and rank possible faults.
- Suggest a diagnostic test that helps distinguish candidate faults.
- Find compatible parts for candidate faults.
- Display the reasoning behind the suggestions.

### Phase 7 — Record repair outcomes

- Let the technician record the confirmed fault, test results, part used, and outcome.
- Save the repair record.
- Update graph weights using a clearly defined rule.
- Keep the update rule explainable and test it with sample cases.

### Phase 8 — Finish the console interface and test

- Add a clear menu.
- Handle unknown devices and symptoms.
- Test common, ambiguous, and unknown symptom combinations.
- Test graph operations and ranking separately.
- Check that saved data can be loaded again.

## 8. Example user workflow

1. The technician chooses a device model.
2. The technician enters one or more symptoms.
3. RepairGraph finds the matching symptom nodes.
4. The graph algorithms explore related faults and diagnostic tests.
5. The ranking logic displays likely faults in order.
6. The program suggests a next test and relevant compatible parts.
7. The technician performs the real-world checks and repair.
8. The technician records the confirmed result.
9. The program saves the case and updates its repair knowledge according to the chosen learning rule.

All example records in the prototype should be synthetic or technician-reviewed. The program should not present a ranking as a guaranteed diagnosis.

## 9. Tools and technologies

- **Programming language:** C++
- **Development environment:** Visual Studio Code
- **Data storage:** Plain text or CSV files loaded into memory when the program starts
- **Graph representation:** Adjacency lists
- **Algorithms and structures:** BFS, DFS, an appropriate weighted-path method, heaps, hash tables, and optional search indexes
- **Testing:** Small test graphs and synthetic diagnosis scenarios

## 10. Expected outcome

The expected outcome is a working console-based prototype for one repair category. It will accept a device and symptoms, rank possible faults, suggest a next diagnostic test, show relevant compatible parts, explain its recommendations, and record repair outcomes.

The project demonstrates how graph modelling, graph traversal, searching, and ranking can be combined in a practical C++ application.

## 11. Risks and limitations

- The quality of recommendations depends on the quality and coverage of the repair examples.
- Synthetic examples cannot represent every real repair scenario.
- Similar symptoms can have different causes.
- Compatibility information must be reviewed; the program should not assume that a part fits every device.
- Dijkstra's algorithm requires non-negative edge costs and solves a shortest-path problem, not automatically a "most likely fault" problem. The scoring model must be designed carefully.
- The system is a decision-support prototype and does not replace technician inspection or professional judgment.

## 12. Future scope

Possible future improvements include:
- Supporting more device categories and models.
- Adding larger, validated repair datasets.
- Improving the fault-ranking model.
- Integrating parts catalogues.
- Adding a graphical interface.
- Adapting the system for multiple repair businesses.

---

# Learning plan: how we will work together

The project will be built in small milestones. You will write the implementation yourself. At each milestone, we will:

1. Learn the relevant DSA concept in simple terms.
2. Understand why it is useful in RepairGraph.
3. Work through a small example by hand.
4. Write pseudocode before C++.
5. You implement the code.
6. Review your code, debug it, and test it.
7. Move to the next milestone only when you can explain the current one.

**Recommended starting point:** Choose whether the prototype will focus on laptop repairs or smartphone repairs. Then we can design the first few node types and relationships before writing the graph class.