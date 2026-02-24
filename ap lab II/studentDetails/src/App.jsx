import { useState } from "react";
import "./App.css";

function App() {
  const [rollNumber, setRollNumber] = useState("");
  const [rollList, setRollList] = useState([]);

  const addRollNumber = () => {
    if (rollNumber.trim() === "") return;
    setRollList([...rollList, rollNumber]);
    setRollNumber("");
  };

  const deleteRollNumber = (index) => {
    const updatedList = rollList.filter((_, i) => i !== index);
    setRollList(updatedList);
  };

  return (
    <div className="container">
      <h1 className="main-title">Student Roll Number Manager</h1>

      <div className="input-section">
        <input
          type="text"
          placeholder="Enter Roll Number"
          value={rollNumber}
          onChange={(e) => setRollNumber(e.target.value)}
        />
        <button onClick={addRollNumber}>Add</button>
      </div>

      <div className="list-section">
        <h2>List of Roll Numbers</h2>

        {rollList.length === 0 ? (
          <p className="empty">No roll numbers added yet.</p>
        ) : (
          <ul>
            {rollList.map((roll, index) => (
              <li key={index}>
                <span>{roll}</span>
                <button
                  className="delete-btn"
                  onClick={() => deleteRollNumber(index)}
                >
                  Delete
                </button>
              </li>
            ))}
          </ul>
        )}
      </div>
    </div>
  );
}

export default App;