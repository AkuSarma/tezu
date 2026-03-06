db.product_details.insertOne({
  product_id: 1,
  description: "High performance wireless gaming mouse",
  images: [
    "mouse_front.jpg",
    "mouse_side.jpg",
    "mouse_back.jpg"
  ],
  attributes: {
    brand: "LogiTech",
    dpi: 16000,
    connectivity: "Wireless",
    weight: "95g"
  },
  reviews: [
    {
      user_id: 3,
      rating: 5,
      comment: "Excellent mouse for gaming!",
      timestamp: new Date()
    },
    {
      user_id: 7,
      rating: 4,
      comment: "Good performance but slightly expensive.",
      timestamp: new Date()
    }
  ]
});