import { useState, useEffect } from 'react'
import './App.css'
import Home from './Page/Home'
import Modal from './Page/Modal.jsx';

const proto = require("./proto/database_service_grpc_web_pb.js"); // Import gRPC client
const messages = require("./proto/database_service_pb.js");
//console.log(proto);
const client = new proto.DatabaseServiceClient("http://localhost:8080", null, null);
function App() {
  const [records, setRecords] = useState([]);
  const [isModalOpen, setIsModalOpen] = useState(false);

  const fetchRecords = () => {
    const request = new messages.StreamRequest();
    request.setMessage("Hi");
    client.getRecords(request, {}, (err, response) => {
      if (err) {
        console.error("Error fetching records:", err);
        return;
      }
      if (!isModalOpen) {
        const newRecords = response.getRecordsList().map((record) => ({
          id: record.getId(),
          name: record.getName(),
        }));
        setRecords(newRecords);
      }
    });
  };

  useEffect(() => {
    

    fetchRecords(); // Initial fetch
  }, [isModalOpen, fetchRecords]);

  

  const handleModal = () => {
    setIsModalOpen(true);
  }

  const deleteHandler = ({id, name}) => {
    const request = new messages.RecordRequest();
    request.setId(id);
    request.setName(name);
    client.deleteRecord(request, {}, (err, response) => {
      if (err) {
        console.error("Error:", err);
      } else {
        console.log("Response from server!");
      }
      fetchRecords();
    });
    
  }

  const saveHandler = ({id, name}) => {
    const request = new messages.RecordRequest();
    request.setId(id);
    request.setName(name);
    client.addRecord(request, {}, (err, response) => {
      if (err) {
        console.error("Error:", err);
      } else {
        console.log("Response from server!");
      }
    });
    setIsModalOpen(false);
    fetchRecords();
  }
  return (
    <>
      <Modal isOpen={isModalOpen} onSave={saveHandler} onClose={() => setIsModalOpen(false)}/>
      <Home handleModal={handleModal} records={records} onDelete={deleteHandler}/>
    </>
  )
}

export default App
