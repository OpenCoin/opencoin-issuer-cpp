#!/bin/sh

curl -X POST http://localhost:18080/cddc \
   -H 'Content-Type: application/json' \
   -d '{"type":"request cddc", "cdd_serial": 1, "message_reference": 1 }'
echo -n "=================================================================="

curl -X POST http://localhost:18080/cddc/serial \
   -H 'Content-Type: application/json' \
   -d '{ "message_reference": 100000, "type": "request cdd serial"}'
echo "=================================================================="

curl -X POST http://localhost:18080/mkcs\
   -H 'Content-Type: application/json' \
   -d '
{
  "denominations": [1, 2, 5],
  "message_reference": 100002,
  "mint_key_ids": [],
  "type": "request mint key certificates"
}'
echo -n "=================================================================="

curl -X POST http://localhost:18080/mint \
   -H 'Content-Type: application/json' \
   -d '
{
  "blinds": [
    {
      "blinded_payload_hash": "924edb672c3345492f38341ff86b57181da4c673ef...",
      "mint_key_id": "1ceb977bb531c65f133ab8b0d60862b17369d96",
      "reference": "a0",
      "type": "blinded payload hash"
    },
    {
      "blinded_payload_hash": "95db92e1c46ebea5edec5e508a831263de6fb78b4c...",
      "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
      "reference": "a1",
      "type": "blinded payload hash"
    },
    {
      "blinded_payload_hash": "10afac98ac43eb40e996c621d5db4d2238348e3f74...",
      "mint_key_id": "897a16bf12bd9ba474ef7be0e3a53553a7b4ece8",
      "reference": "a2",
      "type": "blinded payload hash"
    }
  ],
  "message_reference": 100003,
  "transaction_reference": "b2221a58008a05a6c4647159c324c985",
  "type": "request mint"
}'
echo -n "=================================================================="

curl -X POST http://localhost:18080/renew\
   -H 'Content-Type: application/json' \
   -d '
{
  "blinds": [
    {
      "blinded_payload_hash": "7ed0cda1c1b36f544514b12848b8436974b7b9f6c7...",
      "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
      "reference": "b0",
      "type": "blinded payload hash"
    },
    {
      "blinded_payload_hash": "8924dbcf75ab40e3bd3b4d38315722c981fe10946d...",
      "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
      "reference": "b1",
      "type": "blinded payload hash"
    },
    {
      "blinded_payload_hash": "278fc8e4bd861b7206c065004296af57e14963d928...",
      "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
      "reference": "b2",
      "type": "blinded payload hash"
    },
    {
      "blinded_payload_hash": "2995fd1b9e61926d757a516357f9814e20869fe722...",
      "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
      "reference": "b3",
      "type": "blinded payload hash"
    }
  ],
  "coins": [
    {
      "payload": {
        "cdd_location": "https://opencent.org",
        "denomination": 1,
        "issuer_id": "23ed956e629ba35f0002eaf833ea436aea7db5c2",
        "mint_key_id": "1ceb977bb531c65f133ab8b0d60862b17369d96",
        "protocol_version": "https://opencoin.org/1.0",
        "serial": "cd613e30d8f16adf91b7584a2265b1f5",
        "type": "payload"
      },
      "signature": "2ec0af339566b19fb9867b491ce58025dcefcab649...",
      "type": "coin"
    },
    {
      "payload": {
        "cdd_location": "https://opencent.org",
        "denomination": 2,
        "issuer_id": "23ed956e629ba35f0002eaf833ea436aea7db5c2",
        "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
        "protocol_version": "https://opencoin.org/1.0",
        "serial": "78e510617311d8a3c2ce6f447ed4d57b",
        "type": "payload"
      },
      "signature": "6aefa7472518ed0a1ec64971220ce3a3a921a70bb0...",
      "type": "coin"
    },
    {
      "payload": {
        "cdd_location": "https://opencent.org",
        "denomination": 5,
        "issuer_id": "23ed956e629ba35f0002eaf833ea436aea7db5c2",
        "mint_key_id": "897a16bf12bd9ba474ef7be0e3a53553a7b4ece8",
        "protocol_version": "https://opencoin.org/1.0",
        "serial": "e4b06ce60741c7a87ce42c8218072e8c",
        "type": "payload"
      },
      "signature": "72da93670f666c529f26fcf15092a63c0fa48c8387...",
      "type": "coin"
    }
  ],
  "message_reference": 100004,
  "transaction_reference": "ad45f23d3b1a11df587fd2803bab6c39",
  "type": "request renew"
}'
echo -n "=================================================================="

curl -X POST http://localhost:18080/resume\
   -H 'Content-Type: application/json' \
   -d '
{
  "message_reference": 100005,
  "transaction_reference": "ad45f23d3b1a11df587fd2803bab6c39",
  "type": "request resume"
}'
echo -n "=================================================================="

curl -X POST http://localhost:18080/redeem\
   -H 'Content-Type: application/json' \
   -d '
{
  "coins": [
    {
      "payload": {
        "cdd_location": "https://opencent.org",
        "denomination": 2,
        "issuer_id": "23ed956e629ba35f0002eaf833ea436aea7db5c2",
        "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
        "protocol_version": "https://opencoin.org/1.0",
        "serial": "cd447e35b8b6d8fe442e3d437204e52d",
        "type": "payload"
      },
      "signature": "11b6bfa18134c300f4440df1db17a08fa71a071b71...",
      "type": "coin"
    },
    {
      "payload": {
        "cdd_location": "https://opencent.org",
        "denomination": 2,
        "issuer_id": "23ed956e629ba35f0002eaf833ea436aea7db5c2",
        "mint_key_id": "f2864e5cd937dbaa4825e73a81062de162143682",
        "protocol_version": "https://opencoin.org/1.0",
        "serial": "5b6e6e307d4bedc51431193e6c3f339",
        "type": "payload"
      },
      "signature": "a6dd7b7f1f12c4e411289e8ea0355f24a8597bbc38...",
      "type": "coin"
    }
  ],
  "message_reference": 100006,
  "type": "request redeem"
}'
echo -n "=================================================================="
